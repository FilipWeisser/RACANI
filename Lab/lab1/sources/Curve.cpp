#include "Curve.h"

Curve::Curve(glm::vec3 c, Shader* s, std::vector<GLint> u)
{
	color = c;
	shader = s;
	uniform_variable_location = u;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}
Curve::Curve(glm::vec3 c, Shader* s, std::vector<GLint> u, std::vector<glm::vec3> v)
{
	vertices = v;
	color = c;
	shader = s;
	uniform_variable_location = u;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	refreshBuffers();
}
void Curve::addVertex(glm::vec3 v)
{
	vertices.push_back(v);
	refreshBuffers();
}

void Curve::setVertices(std::vector<glm::vec3> v)
{
	vertices = v;
	refreshBuffers();
}

void Curve::refreshBuffers()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices.size(), (void*)(&vertices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

std::vector<glm::vec3> Curve::getVertices()
{
	return vertices;
}

void Curve::clearVertices()
{
	vertices.clear();
}
