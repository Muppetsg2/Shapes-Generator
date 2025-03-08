#pragma once
#include "Shape.h"

class Pyramid : public Shape {
public:
	Pyramid(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Pyramid();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};