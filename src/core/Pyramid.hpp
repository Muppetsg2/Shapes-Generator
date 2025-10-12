#pragma once
#include "Shape.hpp"
#include <string>

class Pyramid : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Pyramid(const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Pyramid();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};