#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>   
#include <assimp/scene.h>     

#include <Vector>
#include "Transform.h"

class Mesh
{
public:
	//mreza poligona
	std::vector<glm::vec4> vertices;
	std::vector<unsigned int> indices;

	//VAO objekti
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	Mesh(aiMesh* aimesh);
	~Mesh();
	void refreshBuffers();
	glm::mat3 getBoundingBox();
	void applyTransform(glm::mat4 transformation);
	void normalize();
};

