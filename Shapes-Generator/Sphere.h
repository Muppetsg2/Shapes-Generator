#pragma once
#include "Shape.h"

class Sphere : public Shape {
private:
	void _generate(unsigned int h, unsigned int v, ValuesRange range);

public:
	Sphere(unsigned int h = 2u, unsigned int v = 3u, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Sphere();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};