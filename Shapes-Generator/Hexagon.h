#pragma once

#include "Shape.h"

class Hexagon : public Shape {
private:
	void generateCircle(unsigned int segments, float y, unsigned int cullFace);
public:
	Hexagon();
	virtual ~Hexagon();
};