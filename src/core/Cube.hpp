#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Cube : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Cube(const ShapeConfig& config, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cube();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};