#pragma once
#include "Object.h"
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"


Object::Object(Mesh* _mesh, Shader* _shader, std::vector<GLint> _uniform, glm::vec3 _color) {
	mesh = _mesh;
	shader = _shader;
	uniform_variable_location = _uniform;
	color = _color;
}


