#pragma once
#include "Shape.hpp"
#include <string>

class Tetrahedron : public Shape {
private:
	void _generate(const ValuesRange range);

public:
	Tetrahedron(const ShapeConfig& config, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Tetrahedron();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};