#pragma once

#include "Shape.h"

using namespace std;

class Hexagon : public Shape {
private:
	void generateCircle(unsigned int segments, float y, unsigned int cullFace);
public:
	Hexagon();
	virtual ~Hexagon();
};