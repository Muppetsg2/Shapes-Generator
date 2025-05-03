#pragma once
#include "Shape.hpp"

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

	void _generateCircle(unsigned int segments, float y, CylinderCullFace cullFace, ValuesRange range);

protected:
	void _generate(unsigned int horizontalSegments, unsigned int verticalSegments, ValuesRange range, bool useFlatShading);

public:
	Cylinder(unsigned int horizontalSegments = 1u, unsigned int verticalSegments = 3u, CylinderShading shading = CylinderShading::FLAT, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cylinder();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};