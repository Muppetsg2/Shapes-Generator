#pragma once
#include "Shape.hpp"

enum class TorusShading {
	FLAT = 0,
	SMOOTH = 1
};

class Torus : public Shape {
private:
	glm::vec3 _getAverageNormal(const glm::vec3 n1, const glm::vec3 n2, const glm::vec3 n3) const;
	void _generate(const unsigned int segments, const unsigned int cs_segments, const float radius, const float cs_radius, const ValuesRange range, const bool useFlatShading);

public:
	// segments -> co-planar circular axis resolution
	// cs_segments -> revolving circle resolution
	// radius -> co-planar circular axis radius
	// cs_radius -> revolving circle radius
	Torus(const unsigned int segments = 3u, const unsigned int cs_segments = 3u, const float radius = 1.f, const float cs_radius = 0.5f, const TorusShading shading = TorusShading::SMOOTH, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Torus();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};