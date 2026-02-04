#pragma once
#include "Shape.hpp"
#include <string>

class Cube : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Cube(const ShapeConfig& config, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cube();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};