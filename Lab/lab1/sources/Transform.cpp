#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Standard Headers
#include <iostream>


Transform::Transform() {
	position = glm::vec3(0);
	size = 1;
	front = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
}

void Transform::setPosition(glm::vec3 pos) {
	position = pos;
}
void Transform::setOrientation3v(glm::vec3 _front, glm::vec3 _up, glm::vec3 _right) {
	front = _front;
	up = _up;
	right = _right;
}
void Transform::setSize(float _size) {
	size = _size;
}

void Transform::move(glm::vec3 direction) {
	position += direction.z * front;
	position += direction.y * up;
	position += direction.x * right;
}

void Transform::rotate(float angle, glm::vec3 axis) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

	front = glm::normalize(glm::vec3(rotation * glm::vec4(front, 0.0f)));
	up = glm::normalize(glm::vec3(rotation * glm::vec4(up, 0.0f)));
	right = glm::normalize(glm::vec3(rotation * glm::vec4(right, 0.0f)));
}

glm::mat4 Transform::getModelMatrix() {
	return glm::mat4(glm::vec4(front * size, 0.0f),
		glm::vec4(up * size, 0.0f),
		glm::vec4(-right * size, 0.0f),
		glm::vec4(position, 1.0f));
}

glm::vec3 Transform::getPosition() {
	return position;
}

glm::vec3 Transform::getOrientationFront()
{
	return -right;
}
