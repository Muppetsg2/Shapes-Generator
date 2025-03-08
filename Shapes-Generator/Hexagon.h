#pragma once
#include "Shape.h"

class Hexagon : public Shape {
private:
	enum class HexagonCullFace {
		FRONT = 0,
		BACK = 1
	};

	void generateCircle(unsigned int segments, float y, HexagonCullFace cullFace, ValuesRange range);
public:
	Hexagon(ValuesRange range = ValuesRange::HALF_TO_HALF);
	virtual ~Hexagon();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};