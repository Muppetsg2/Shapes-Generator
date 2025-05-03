#pragma once
#include "Shape.hpp"

enum class TorusShading {
	FLAT = 0,
	SMOOTH = 1
};

class Torus : public Shape {
private:
	glm::vec3 _getAverageNormal(glm::vec3 n1, glm::vec3 n2, glm::vec3 n3);
	void _generate(unsigned int segments, unsigned int cs_segments, float radius, float cs_radius, ValuesRange range, bool useFlatShading);

public:
	// segments -> co-planar circular axis resolution
	// cs_segments -> revolving circle resolution
	// radius -> co-planar circular axis radius
	// cs_radius -> revolving circle radius
	Torus(unsigned int segments = 3u, unsigned int cs_segments = 3u, float radius = 1.f, float cs_radius = 0.5f, TorusShading shading = TorusShading::SMOOTH, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Torus();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};