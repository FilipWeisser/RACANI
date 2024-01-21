#pragma once
#include "Transform.h"
#include "Curve.h"

#include <glm/glm.hpp>

class Camera : public Transform
{
private:
	float pitch, yaw;
public:
	Camera();
	float getPitch();
	float getYaw();
	void setPitch(float p);
	void setYaw(float p);
	glm::vec3 getViewPoint();
	glm::mat4 getPerspectiveMatrix(int height, int width);
	glm::mat4 getViewMatrix();
	void animate(Curve curve);
};

