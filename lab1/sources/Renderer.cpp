#include "Renderer.h"
#include "Object.h"
#include "Transform.h"
#include "Bezier.h"
#include <Vector>
#include <iostream>
//#include <glad/glad.h>

Renderer::Renderer(Camera* _camera) : camera(_camera) {};

void Renderer::addObject(Object* object) {
	objects.push_back(object);
}

void Renderer::addCurve(Curve* curve)
{
	curves.push_back(curve);
}

void Renderer::render(int height, int width) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//****objekti****//
	for (int i = 0; i < objects.size(); i++) {
		//slanje uniformnih varijabli
		glUseProgram(objects[i]->shader->ID);
		glUniform3f(objects[i]->uniform_variable_location[0], objects[i]->color.x, objects[i]->color.y, objects[i]->color.z);
		glUniformMatrix4fv(objects[i]->uniform_variable_location[1], 1, GL_FALSE, &camera->getPerspectiveMatrix(height, width)[0][0]);
		glUniformMatrix4fv(objects[i]->uniform_variable_location[2], 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(objects[i]->uniform_variable_location[3], 1, GL_FALSE, &objects[i]->getModelMatrix()[0][0]);
		if (objects[i]->uniform_variable_location.size() > 4) {
			glm::vec3 eye = camera->getPosition();
			glUniform3f(objects[i]->uniform_variable_location[4], eye.x, eye.y, eye.z);
		}

		//crtanje
		glBindVertexArray(objects[i]->mesh->VAO);
		glDrawElements(GL_TRIANGLES, objects[i]->mesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	//****krivulje****//
	for (int i = 0; i < curves.size(); i++) {
		if (curves[i]->getVertices().empty()) continue;
		glUseProgram(curves[i]->shader->ID);
		glUniform3f(curves[i]->uniform_variable_location[0], curves[i]->color.x, curves[i]->color.y, curves[i]->color.z);
		glUniformMatrix4fv(curves[i]->uniform_variable_location[1], 1, GL_FALSE, &camera->getPerspectiveMatrix(height, width)[0][0]);
		glUniformMatrix4fv(curves[i]->uniform_variable_location[2], 1, GL_FALSE, &camera->getViewMatrix()[0][0]);

		//crtanje
		glBindVertexArray(curves[i]->VAO);
		glDrawArrays(GL_LINE_STRIP, 0, curves[i]->getVertices().size());
		glBindVertexArray(0);
	}

}
