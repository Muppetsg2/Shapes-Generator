#pragma once
#include "Cylinder.h"

class Hexagon : public Cylinder {
public:
	Hexagon(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Hexagon();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};