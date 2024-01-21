// Local Headers
#include "Transform.h"
#include "Object.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "FPSManager.h"
#include "Curve.h"
#include "Bezier.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Headers
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace glm;

int width = 1000, height = 1000;
double cursor_x = width/2;
double cursor_y = height/2;
float camera_speed = 0.7;
float sensitivity = 0.03;
Camera* camera = new Camera();

float p = 1.6;
bool animate = 0;

Shader* loadShader(char* path, char* naziv, bool geometry) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathGeom;
	std::string pathFrag;

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathGeom.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathGeom.append("shaders/");
		pathFrag.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathGeom.append("shaders\\");
		pathFrag.append("shaders\\");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathGeom.append(naziv);
	pathGeom.append(".geom");
	pathFrag.append(naziv);
	pathFrag.append(".frag");

	if (!geometry) 	return new Shader(pathVert.c_str(), pathFrag.c_str());
	return new Shader(pathVert.c_str(), pathGeom.c_str(), pathFrag.c_str());
}
void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
	width = Width;
	height = Height;

	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			camera->move(glm::vec3(0, 0, 1) * camera_speed);
			break;
		case GLFW_KEY_S:
			camera->move(glm::vec3(0, 0, -1) * camera_speed);
			break;
		case GLFW_KEY_A:
			camera->move(glm::vec3(-1, 0, 0) * camera_speed);
			break;
		case GLFW_KEY_D:
			camera->move(glm::vec3(1, 0, 0) * camera_speed);
			break;
		case GLFW_KEY_E:
			camera->move(glm::vec3(0, -1, 0) * camera_speed);
			break;
		case GLFW_KEY_Q:
			camera->move(glm::vec3(0, 1, 0) * camera_speed);
			break;

		case GLFW_KEY_SPACE:
			animate = !animate;
			break;
		}
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
	float xoffset = (xpos - cursor_x) * sensitivity;
	float yoffset = (cursor_y - ypos) * sensitivity;
	cursor_x = xpos;
	cursor_y = ypos;

	camera->setYaw(camera->getYaw() + xoffset);
	float pitch = camera->getPitch() + yoffset;

	if (pitch > 89.0f) camera->setPitch(89.0f);
	else if (pitch < -89.0f) camera->setPitch(-89.0f);
	else camera->setPitch(pitch);

	glm::vec3 direction;
	direction.x = cos(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
	direction.y = sin(glm::radians(camera->getPitch()));
	direction.z = sin(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
	camera->setOrientation3v(glm::normalize(direction), glm::vec3(0, 1, 0), glm::normalize(glm::cross(glm::normalize(direction), glm::vec3(0, 1, 0))));
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		animate = !animate;
	}

}

int main(int argc, char* argv[]) {

	glfwInit();
	gladLoadGL();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Zadatak 7", nullptr, nullptr);
	FPSManager FPSManagerObject(window, 60, 1.0, "Zadatak 7");

	// provjeri je li se uspio napraviti prozor
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// dohvati sve dostupne OpenGL funkcije
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); //ukljuci z spremnik (prikazuju se oni fragmenti koji su najblizi promatracu)
	glDepthFunc(GL_LESS);
	glClearColor(1, 1, 1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira
	glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	

	//obj import
	Assimp::Importer importer;

	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("\\resources");
	std::string objPath(resPath);
	objPath.append("\\f16.obj");

	const aiScene* scene = importer.ReadFile(objPath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::cerr << importer.GetErrorString();
		return false;
	}

	if (scene->HasMeshes()) {
		aiMesh* aimesh = scene->mMeshes[0];

		Renderer* renderer = new Renderer(camera);

		//loading shaders and locations of uniform variables
		Shader* shader = loadShader(argv[0], "shader", false);
		std::vector<GLint> uniform_variable_location = {
			glGetUniformLocation(shader->ID, "u_color") ,
			glGetUniformLocation(shader->ID, "u_projection_mat") ,
			glGetUniformLocation(shader->ID, "u_view_mat") ,
		};
		Shader* shader1 = loadShader(argv[0], "shader1", true);
		std::vector<GLint> uniform_variable_location1 = {
			glGetUniformLocation(shader1->ID, "u_color") ,
			glGetUniformLocation(shader1->ID, "u_projection_mat") ,
			glGetUniformLocation(shader1->ID, "u_view_mat") ,
			glGetUniformLocation(shader1->ID, "u_model_mat") ,
			glGetUniformLocation(shader1->ID, "u_eye")
		};

		//creating objects
		Object* object = new Object(new Mesh(aimesh), shader1, uniform_variable_location1, glm::vec3(0.4, 0.3, 0.7));

		//spline
		std::string splinePath(resPath);
		splinePath.append("\\spline.obj");
		const aiScene* sceneSpline = importer.ReadFile(splinePath.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!sceneSpline) {
			std::cerr << importer.GetErrorString();
			return false;
		}

		vector<vec3> splineControlPoints;
		const aiMesh* meshSpline = sceneSpline->mMeshes[0];
		for (int i = 0; i < meshSpline->mNumVertices; i++) {
			const aiVector3D& vertex = meshSpline->mVertices[i];
			splineControlPoints.push_back(vec3(vertex.x, vertex.y, vertex.z));
			//std::cout << "Vertex " << i << ": (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
		}

		vector<vec3> splinePoints;
		vector<vec3> splinePointTangents;
		for (int i = 0; i < splineControlPoints.size() - 3; i++) {
			vec3 r0 = splineControlPoints[i];
			vec3 r1 = splineControlPoints[i + 1];
			vec3 r2 = splineControlPoints[i + 2];
			vec3 r3 = splineControlPoints[i + 3];
			for (float t = 0.0; t <= 1.0; t += 0.01) {

				float f1 = (-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)/6;
				float f2 = (3 * pow(t, 3) + -6 * pow(t, 2) + 4)/6;
				float f3 = (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)/6;
				float f4 = pow(t, 3)/6;

				vec3 point;
				point.x = f1 * r0.x + f2 * r1.x + f3 * r2.x + f4 * r3.x;
				point.y = f1 * r0.y + f2 * r1.y + f3 * r2.y + f4 * r3.y;
				point.z = f1 * r0.z + f2 * r1.z + f3 * r2.z + f4 * r3.z;
				splinePoints.push_back(point);


				float t1 = (-1 * pow(t, 2) + 2 * t - 1)/2;
				float t2 = (3 * pow(t, 2) - 4 * t)/2;
				float t3 = (-3 * pow(t, 2) + 2 * t + 1)/2;
				float t4 = pow(t, 2)/2;

				vec3 tangent;
				tangent.x = t1 * r0.x + t2 * r1.x + t3 * r2.x + t4 * r3.x;
				tangent.y = t1 * r0.y + t2 * r1.y + t3 * r2.y + t4 * r3.y;
				tangent.z = t1 * r0.z + t2 * r1.z + t3 * r2.z + t4 * r3.z;

				splinePointTangents.push_back(normalize(tangent));

			}
		}
		Curve* spline = new Curve(glm::vec3(0, 0, 1), shader, uniform_variable_location, splinePoints);


		//scaling, moving and rotating objects
		object->setSize(2);
		camera->setPosition(vec3(0, 10, -10));

		vec3 front = splinePointTangents[0];
		vec3 right = normalize(cross(front, vec3(0, 0, 1)));

		object->setPosition(splinePoints[0]);
		object->setOrientation3v(right, normalize(cross(-front, right)), -front);
		
		
		renderer->addObject(object);
		renderer->addCurve(spline);


		int current_step = 0;
		vec3 front2;
		vec3 translation;
		while (glfwWindowShouldClose(window) == false) {
			//pobrisi platno
			float deltaTime = (float)FPSManagerObject.enforceFPS(false);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderer->render(height, width);
			
			//tangente
			glBegin(GL_LINES);
			for (int i = 0; i < splinePointTangents.size(); i += 20) {
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(splinePoints[i].x, splinePoints[i].y, splinePoints[i].z);
				glVertex3f(splinePoints[i].x + splinePointTangents[i].x*p, 
					splinePoints[i].y + splinePointTangents[i].y*p, 
					splinePoints[i].z + splinePointTangents[i].z*p);
			}
			glEnd();


			//animacija
			if (animate) {
				if (current_step > splinePoints.size() - 3) {
					//pocetni polozaj
					current_step = 0;
					object->setPosition(splinePoints[0]);
					object->setOrientation3v(right, normalize(cross(-front, right)), -front);
				}
				//pomak
				translation = splinePoints[current_step+1] - splinePoints[current_step];
				object->move(-vec3(length(translation), 0,0));
				
				//rotacija
				front2 = splinePointTangents[current_step+1];
				float dotProduct = dot(object->getOrientationFront(), front2);
				vec3 rotationAxis = normalize(cross(object->getOrientationFront(), front2));

				float angle = acos(dotProduct);
				object->rotate(angle*180.0/pi<float>(), rotationAxis);

				current_step++;
			}

			glfwSwapBuffers(window);
			glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		delete renderer, aimesh, object, shader, shader1;
	}
	return EXIT_SUCCESS;
}