#pragma once
#include "Cylinder.hpp"
#include "Shape.hpp"
#include <string>

class Hexagon : public Cylinder {
public:
	Hexagon(const ShapeConfig& config, const unsigned int segments = 1u, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Hexagon();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};