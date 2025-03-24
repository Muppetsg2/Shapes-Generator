#pragma once
#include "Shape.h"

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

	void generateCircle(unsigned int segments, float y, CylinderCullFace cullFace, ValuesRange range);

protected:
	void generate(unsigned int segments, ValuesRange range, bool useFlatShading);

public:
	Cylinder(unsigned int segments = 3u, CylinderShading shading = CylinderShading::FLAT, ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Cylinder();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};