#pragma once
#include <Vector>
#include <glm/glm.hpp>
#include <glad/glad.h> 

class Bezier {

public:
	static std::vector<glm::vec3> approximation_curve(std::vector<glm::vec3> points, int number_of_points);
	static std::vector<glm::vec3> interpolation_curve(std::vector<glm::vec3> points, int number_of_points);

};
