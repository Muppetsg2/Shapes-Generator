// PRECOMPILED HEADER
#include "pch.h"
#include "Hexagon.h"

Hexagon::Hexagon(unsigned int segments, ValuesRange range)
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