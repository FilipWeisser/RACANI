#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() {
	//setPosition(glm::vec3(0, 0, 5));
	setOrientation3v(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));
	pitch = 0;
	yaw = 90;
}
float Camera::getPitch() {
	return pitch;
}
float Camera::getYaw() {
	return yaw;
}
void Camera::setPitch(float p) {
	pitch = p;
}
void Camera::setYaw(float y) {
	yaw = y;
}

glm::vec3 Camera::getViewPoint() {
	return glm::vec3(position+front*3.f);
}

glm::mat4 Camera::getPerspectiveMatrix(int height, int width) {
	return glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.f, 100.0f);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}
