#pragma once
#include "Shape.h"

enum class PlaneNormalDir {
	UP = 0,
	FRONT = 1
};

class Plane : public Shape {
private:
	float fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd);

	void generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir, ValuesRange range);

public:
	Plane(unsigned int rows = 2u, unsigned int columns = 2u, PlaneNormalDir dir = PlaneNormalDir::UP, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Plane();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};