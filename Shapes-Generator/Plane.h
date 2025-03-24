#pragma once
#include "Shape.h"

enum class PlaneNormalDir {
	UP = 0,
	FRONT = 1
};

class Plane : public Shape {
private:
	void generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir, ValuesRange range);

public:
	Plane(unsigned int rows = 2u, unsigned int columns = 2u, PlaneNormalDir dir = PlaneNormalDir::UP, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Plane();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};