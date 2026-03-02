#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <string>
#pragma endregion

#pragma region MY_FILES
#include "Hexagon.hpp"
#include "Shape.hpp"
#pragma endregion

Hexagon::Hexagon(const ShapeConfig& config, const unsigned int segments, const ValuesRange range)
{
    _shapeConfig = config;
    _vertices.clear();
    _indices.clear();
    _generate(segments, 6u, range, true);
}

Hexagon::~Hexagon() {}

std::string Hexagon::getClassName()
{
    return "Hexagon";
}

std::string Hexagon::getObjectClassName() const
{
    return Hexagon::getClassName();
}