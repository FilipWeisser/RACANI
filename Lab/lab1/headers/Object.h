#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"

class Object : public Transform
{
public:
	Mesh* mesh;
	Shader* shader;
	std::vector<GLint> uniform_variable_location;
	glm::vec3 color;

	Object(Mesh* mesh, Shader* shader, std::vector<GLint> uniform, glm::vec3 color);
};

