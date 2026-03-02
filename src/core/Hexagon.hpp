#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Cylinder.hpp"
#include "Shape.hpp"
#pragma endregion

class Hexagon : public Cylinder {
public:
	Hexagon(const ShapeConfig& config, const unsigned int segments = 1u, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Hexagon();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};