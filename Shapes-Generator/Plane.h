#pragma once

#include "Shape.h"

using namespace std;

enum PlaneNormalDir {
	UP = 0,
	FRONT = 1
};

class Plane : public Shape {
private:
	float fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd);

	void generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir);

public:
	Plane(unsigned int rows = 2, unsigned int columns = 2, PlaneNormalDir dir = PlaneNormalDir::UP);
	virtual ~Plane();
};