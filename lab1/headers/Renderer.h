#pragma once
#include "Object.h"
#include "Camera.h"
#include "Curve.h"

class Renderer
{
public:
	Camera* camera;
	std::vector<Object*> objects;
	std::vector<Curve*> curves;

	Renderer(Camera* camera);
	void addObject(Object* object);
	void addCurve(Curve* curve);
	void render(int height, int width);
	void render_curve(Curve* curve, int number_of_points);
};

