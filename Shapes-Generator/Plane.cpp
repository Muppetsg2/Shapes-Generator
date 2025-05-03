#include "pch.hpp"
#include "Plane.hpp"

void Plane::_generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir, ValuesRange range)
{
    float space = range == ValuesRange::HALF_TO_HALF ? 1.f : 2.f;
    float minRange = -space * .5f;
    float maxRange = space * .5f;
    
    float diffX = space / (float)(columns - 1u);
    float diffZ = space / (float)(rows - 1u);

    std::vector<unsigned int> trisNum;

    for (unsigned int row = 0u; row < rows; ++row) {
        float z = minRange + (float)row * diffZ;

        for (unsigned int col = 0u; col < columns; ++col) {
            float x = minRange + (float)col * diffX;

            switch (dir) {
                case PlaneNormalDir::UP : {
                    _vertices.push_back({ { x, 0.f, z }, { _map(x, minRange, maxRange, 0.f, 1.f), _map(z, minRange, maxRange, 0.f, 1.f) }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
                case PlaneNormalDir::FRONT : {
                    _vertices.push_back({ { x, z, 0.f }, { _map(x, minRange, maxRange, 0.f, 1.f), _map(z, minRange, maxRange, 0.f, 1.f) }, { 0.f, 0.f, -1.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
                default : {
                    _vertices.push_back({ { x, 0.f, z }, { _map(x, minRange, maxRange, 0.f, 1.f), _map(z, minRange, maxRange, 0.f, 1.f) }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
            }

            if (row * columns + col == 0u || row * columns + col == rows * columns - 1u) {
                trisNum.push_back(1u);
            }
            else if ((row == 0u && col + 1u == columns) || (row + 1u == rows && col == 0u)) {
                trisNum.push_back(2u);
            }
            else if (row == 0u || row + 1u == rows || col == 0u || col + 1u == columns) {
                trisNum.push_back(3u);
            }
            else {
                trisNum.push_back(6u);
            }
        }
    }

    const size_t vertSize = _vertices.size();
    for (size_t i = 0ull; i < vertSize; ++i) {

        size_t first = i + (size_t)columns;
        size_t second = i + 1ull;

        if (second % (size_t)columns == 0ull) continue;

        if (first >= vertSize) break;

        // First Triangle
        size_t third = i;

        _indices.push_back((unsigned int)first);
        _indices.push_back((unsigned int)second);
        _indices.push_back((unsigned int)third);

        std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent((unsigned int)first, (unsigned int)second, (unsigned int)third);

        _vertices[first].Tangent += TB.first;
        _vertices[first].Bitangent += TB.second;

        _vertices[second].Tangent += TB.first;
        _vertices[second].Bitangent += TB.second;

        _vertices[third].Tangent += TB.first;
        _vertices[third].Bitangent += TB.second;

        // Second Triangle
        third = second;
        second = first + 1ull;

        _indices.push_back((unsigned int)first);
        _indices.push_back((unsigned int)second);
        _indices.push_back((unsigned int)third);

        TB = _calcTangentBitangent((unsigned int)first, (unsigned int)second, (unsigned int)third);

        _vertices[first].Tangent += TB.first;
        _vertices[first].Bitangent += TB.second;

        _vertices[second].Tangent += TB.first;
        _vertices[second].Bitangent += TB.second;

        _vertices[third].Tangent += TB.first;
        _vertices[third].Bitangent += TB.second;
    }

    _normalizeTangents(trisNum, 0ull, vertSize);

    trisNum.clear();
}

Plane::Plane(unsigned int rows, unsigned int columns, PlaneNormalDir dir, ValuesRange range)
{
    _vertices.clear();
    _indices.clear();
    _generate(std::max(2u, rows), std::max(2u, columns), dir, range);
}

Plane::~Plane() {}

std::string Plane::getClassName()
{
    return "Plane";
}

std::string Plane::getObjectClassName() const
{
    return Plane::getClassName();
}