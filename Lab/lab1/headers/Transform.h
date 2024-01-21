#pragma once
#include "glm/glm.hpp"

class Transform
{
protected:
	//pozicija
	glm::vec3 position;
	//velicina
	float size;
	//orijentacija
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	
public:
	Transform();
	void setPosition(glm::vec3 pos);
	void setOrientation3v(glm::vec3 front, glm::vec3 up, glm::vec3 right);
	void setSize(float size);

	glm::vec3 getPosition();
	glm::vec3 getOrientationFront();

	void move(glm::vec3 direction);
	void rotate(float angle, glm::vec3 axis);

	glm::mat4 getModelMatrix();


	static glm::mat4 translate3D(glm::vec3 translateVector) {
		return glm::transpose(glm::mat4(1, 0, 0, translateVector.x,
			0, 1, 0, translateVector.y,
			0, 0, 1, translateVector.z,
			0, 0, 0, 1));
	}
	static glm::mat4 scale3D(glm::vec3 scaleVector) {
		return glm::transpose(glm::mat4(scaleVector.x, 0, 0, 0,
			0, scaleVector.y, 0, 0,
			0, 0, scaleVector.z, 0,
			0, 0, 0, 1));
	}
	/*
	static glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {
		glm::vec3 f = glm::normalize(center - eye);
		viewUp = glm::normalize(viewUp);
		glm::vec3 s = glm::normalize(glm::cross(f, viewUp));
		glm::vec3 u = glm::cross(s, f);

		glm::mat4 M = glm::mat4(s.x, s.y, s.z, -glm::dot(s, eye),
			u.x, u.y, u.z, -glm::dot(u, eye),
			-f.x, -f.y, -f.z, glm::dot(f, eye),
			0, 0, 0, 1);

		return glm::transpose(M);
	}

	static glm::mat4 frustum(double l, double r, double b, double t, double n, double f) {
		return glm::transpose(glm::mat4(2 * n / (r - l), 0, (r + l) / (r - l), 0,
			0, 2 * n / (t - b), (t + b) / (t - b), 0,
			0, 0, -(f + n) / (f - n), -2 * f * n / (f - n),
			0, 0, -1, 0));
	}*/
};

