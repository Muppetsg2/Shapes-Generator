#pragma once
#include "Shape.hpp"

enum class SphereShading {
	FLAT = 0,
	SMOOTH = 1
};

class Sphere : public Shape {
private:
	glm::vec3 _getAverageNormal(const glm::vec3 n1, const glm::vec3 n2, const glm::vec3 n3) const;
	void _generate(const unsigned int h, const unsigned int v, const ValuesRange range, const bool useFlatShading);

public:
	Sphere(const unsigned int h = 2u, const unsigned int v = 3u, const SphereShading shading = SphereShading::SMOOTH, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Sphere();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};