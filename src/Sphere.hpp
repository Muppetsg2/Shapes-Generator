#pragma once
#include "Shape.hpp"

enum class SphereShading {
	FLAT = 0,
	SMOOTH = 1
};

class Sphere : public Shape {
private:
	glm::vec3 _getAverageNormal(glm::vec3 n1, glm::vec3 n2, glm::vec3 n3);
	void _generate(unsigned int h, unsigned int v, ValuesRange range, bool useFlatShading);

public:
	Sphere(unsigned int h = 2u, unsigned int v = 3u, SphereShading shading = SphereShading::SMOOTH, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Sphere();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};