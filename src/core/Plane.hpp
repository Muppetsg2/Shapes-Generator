#pragma once

#pragma region STD_LIBS
#include <cstdint>
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

enum class PlaneNormalDir : uint8_t {
	UP = 0,
	FRONT = 1
};

class Plane : public Shape {
private:
	void _generate(const unsigned int rows, const unsigned int columns, const PlaneNormalDir dir, const ValuesRange range);

public:
	Plane(const ShapeConfig& config, const unsigned int rows = 2u, const unsigned int columns = 2u, const PlaneNormalDir dir = PlaneNormalDir::UP, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Plane();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};