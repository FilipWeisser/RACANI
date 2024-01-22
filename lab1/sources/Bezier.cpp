#include "Bezier.h"
#include "iostream"
#include "math.h"
#include "algorithm"

std::vector<float> compute_factors(int n) {
	int i;
	float a = 1;
	std::vector<float> factors;
	for (i = 1; i <= n + 1; i++) {
		factors.push_back(a);
		a = a * (n - i + 1) / (float)i;
	}
	return factors;
}

float bezier_recursive(int i, int n, float t) {
	if (i == -1 || (i == 0 && n == 0)) return 1;
	if (i == n + 1) return 0;
	return (1 - t) * bezier_recursive(i, n - 1, t) + t * bezier_recursive(i - 1, n - 1, t);
}

std::vector<glm::vec3> Bezier::approximation_curve(std::vector<glm::vec3> points, int number_of_points)
{
	int i, j, n = (int)points.size() - 1;
	double t;
	std::vector<glm::vec3> curve;
	std::vector<float> factors = compute_factors(n);

	for (i = 0; i <= number_of_points; i++) {
		glm::vec3 point = glm::vec3(0,0,0);
		t = i / (double)number_of_points;
		for (j = 0; j <= n; j++) {
			point += points[j] * factors[j] * (float)pow(t, j) * (float)pow(1 - t, n - j);
		}
		curve.push_back(point);
	}
	return curve;
}

std::vector<glm::vec3> Bezier::interpolation_curve(std::vector<glm::vec3> points, int number_of_points)
{
	std::vector<glm::vec3> curve;
	std::vector<glm::vec3> A;
	glm::vec3 a;
	int i, j, n;
	float t;

	switch (points.size()) {
	case 1:
		return points;

	case 2: {
		for (i = 0; i <= number_of_points; i++) {
			t = i / (float)number_of_points;
			curve.push_back(points[0] * (1 - t) + points[1] * t);
		}
		return curve;
	}
	case 3: {
		n = 2;

		glm::mat3 F;
		for (i = 0; i <= n; i++)
			for (j = 0; j <= n; j++)
				F[i][j] = bezier_recursive(j, n, i / (float)n);
		F = glm::inverse(F);

		for (i = 0; i <= n; i++) {
			a = glm::vec3(0);
			for (j = 0; j <= n; j++)
				a += F[i][j] * points[j];
			A.push_back(a);
		}
		break;
	}
	default: {
		std::vector<glm::vec3> control_points = points;
		if (control_points.size() > 4) {
			control_points.erase(control_points.begin(), control_points.end() - 4);
		}
		n = 3;

		glm::mat4 F;
		for (i = 0; i <= n; i++)
			for (j = 0; j <= n; j++)
				F[i][j] = bezier_recursive(j, n, i / (float)n);

		F = glm::inverse(F);

		for (i = 0; i <= n; i++) {
			a = glm::vec3(0);
			for (j = 0; j <= n; j++)
				a += F[i][j] * control_points[j];
			A.push_back(a);
		}

	}
	}

	glm::vec3 point;
	for (i = 0; i <= number_of_points; i++) {
		point = glm::vec3(0);
		t = i / (float)number_of_points;
		for (j = 0; j <= n; j++)
			point += A[j] * bezier_recursive(j, n, t);
		curve.push_back(point);
	}
	return curve;
}