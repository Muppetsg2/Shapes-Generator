#pragma once
#include "Shape.hpp"
#include <string>

enum class CylinderShading {
	FLAT = 0,
	SMOOTH = 1
};

class Cylinder : public Shape {
private:
	enum class CylinderCullFace {
		FRONT = 0,
		BACK = 1
	};

	void _generateCircle(const unsigned int segments, const float y, const CylinderCullFace cullFace, const ValuesRange range);

protected:
	void _generate(const unsigned int horizontalSegments, const unsigned int verticalSegments, const ValuesRange range, const bool useFlatShading);

public:
	Cylinder(const unsigned int horizontalSegments = 1u, const unsigned int verticalSegments = 3u, const CylinderShading shading = CylinderShading::FLAT, const ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cylinder();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};