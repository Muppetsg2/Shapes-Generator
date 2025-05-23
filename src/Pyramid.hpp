#pragma once
#include "Shape.hpp"

class Pyramid : public Shape {
private:
	void _generate(ValuesRange range);

public:
	Pyramid(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Pyramid();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};