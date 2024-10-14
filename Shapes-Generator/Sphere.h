#pragma once

#include "Shape.h"

class Sphere : public Shape {
private:
	unsigned int _segmentsHorizontal = 2;
	unsigned int _segmentsVertical = 3;

	void updateVerticiesAndIndices();

public:
	Sphere();
	Sphere(unsigned int h, unsigned int v);
	virtual ~Sphere();

	void setSegmentsHorizontal(unsigned int segmentsHorizontal);
	void setSegmentsVertical(unsigned int segmentsVertical);
};