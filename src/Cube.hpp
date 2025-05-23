#pragma once
#include "Shape.hpp"

class Cube : public Shape {
private:
	void _generate(ValuesRange range);

public:
	Cube(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cube();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};