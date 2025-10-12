#include "pch.hpp"
#include "Hexagon.hpp"
#include "Shape.hpp"

#include <string>

Hexagon::Hexagon(const unsigned int segments, const ValuesRange range)
{
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