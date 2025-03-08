#pragma once
#include "Shape.h"

class Cube : public Shape {
public:
	Cube(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cube();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};