#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Tetrahedron : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Tetrahedron(const ShapeConfig& config, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Tetrahedron();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};