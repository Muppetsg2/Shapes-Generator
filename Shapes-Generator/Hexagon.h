#pragma once
#include "Cylinder.h"

class Hexagon : public Cylinder {
public:
	Hexagon(unsigned int segments = 1u, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Hexagon();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};