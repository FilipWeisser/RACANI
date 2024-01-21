#pragma once
#include "Shader.h"

#include <Vector>
#include <glm/glm.hpp>
#include <glad/glad.h> 

class Curve
{
private:
	std::vector<glm::vec3> vertices;

public:
	glm::vec3 color;
	Shader* shader;
	std::vector<GLint> uniform_variable_location;
	GLuint VAO;
	GLuint VBO;

	Curve(glm::vec3 c, Shader* s, std::vector<GLint> u);
	Curve(glm::vec3 c, Shader* s, std::vector<GLint> u, std::vector<glm::vec3> vertices);
	void addVertex(glm::vec3 v);
	void setVertices(std::vector<glm::vec3> v);
	std::vector<glm::vec3> getVertices();
	void clearVertices();

	void refreshBuffers();
};

