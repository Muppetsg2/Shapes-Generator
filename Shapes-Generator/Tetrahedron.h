#pragma once
#include "Shape.h"

class Tetrahedron : public Shape {
public:
	Tetrahedron(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Tetrahedron();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};