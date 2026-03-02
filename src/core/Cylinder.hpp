#pragma once

#pragma region STD_LIBS
#include <cstdint>
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Cylinder : public Shape {
private:
	enum class CylinderCullFace : uint8_t {
		FRONT = 0,
		BACK = 1
	};

	void _generateCircle(const unsigned int segments, const float y, const CylinderCullFace cullFace, const ValuesRange range);

protected:
	void _generate(const unsigned int horizontalSegments, const unsigned int verticalSegments, const ValuesRange range, const bool useFlatShading);

public:
	Cylinder() = default;
	Cylinder(const ShapeConfig& config, const unsigned int horizontalSegments = 1u, const unsigned int verticalSegments = 3u, const ValuesRange range = ValuesRange::HALF_TO_HALF, const Shading shading = Shading::FLAT);
	virtual ~Cylinder();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};