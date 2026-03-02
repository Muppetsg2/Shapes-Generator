#pragma once

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region GLM_LIB
#include <glm/fwd.hpp>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class Sphere : public Shape {
private:
	glm::vec3 _getAverageNormal(const glm::vec3 n1, const glm::vec3 n2, const glm::vec3 n3) const;
	void _generate(const unsigned int h, const unsigned int v, const ValuesRange range, const bool useFlatShading);

public:
	Sphere(const ShapeConfig& config, const unsigned int h = 2u, const unsigned int v = 3u, const ValuesRange range = ValuesRange::HALF_TO_HALF, const Shading shading = Shading::SMOOTH);
	virtual ~Sphere();

	static std::string getClassName();
	std::string getObjectClassName() const override;
};