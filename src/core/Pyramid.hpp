#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Pyramid : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Pyramid(const ShapeConfig& config, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Pyramid();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};