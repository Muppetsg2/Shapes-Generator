#pragma once
#include "Shape.h"

enum class ConeShading {
	FLAT = 0,
	SMOOTH = 1
};

class Cone : public Shape {
private:
	void generate(unsigned int segments, float height, float radius, ValuesRange range, bool useFlatShading);
public:
	Cone(unsigned int segments = 3u, float height = 1.f, float radius = 1.f, ConeShading shading = ConeShading::FLAT, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cone();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};