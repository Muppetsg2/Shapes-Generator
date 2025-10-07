#pragma once
#include "Shape.hpp"

enum class ConeShading {
	FLAT = 0,
	SMOOTH = 1
};

class Cone : public Shape {
private:
	void _generate(const unsigned int segments, const float height, const float radius, const ValuesRange range, const bool useFlatShading);
public:
	Cone(const unsigned int segments = 3u, const float height = 1.f, const float radius = 1.f, const ConeShading shading = ConeShading::FLAT, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cone();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};