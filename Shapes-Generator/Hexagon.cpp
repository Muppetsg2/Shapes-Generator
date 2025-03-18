// PRECOMPILED HEADER
#include "pch.h"
#include "Hexagon.h"

Hexagon::Hexagon(ValuesRange range)
{
    vertices.clear();
    indices.clear();
    generate(6u, range);
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