#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Cone : public Shape {
private:
	void _generate(const unsigned int segments, const float height, const float radius, const ValuesRange range, const bool useFlatShading);
public:
	Cone(const ShapeConfig& config, const unsigned int segments = 3u, const float height = 1.f, const float radius = 1.f, const ValuesRange range = ValuesRange::HALF_TO_HALF, const Shading shading = Shading::FLAT);
	virtual ~Cone();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};