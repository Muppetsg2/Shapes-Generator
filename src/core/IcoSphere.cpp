#include "pch.hpp"
#include "IcoSphere.hpp"
#include "BitMathOperators.hpp"
#include "Constants.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

void IcoSphere::_generateIcoSahedron(const float mult, const bool useFlatShading, const bool hasSubdivisions)
{
    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;

    constexpr float t = .5f + (float)M_SQRT5 * .5f;

    for (unsigned int i = 0u; i < 3u; ++i) {
        for (unsigned int j = 0u; j < 4u; ++j) {
            const float a = t * (float)(1 - (int)mul_2(div_2(j)));
            const float b = (float)(1 - (int)mul_2(mod_2(j + 1u)));
            glm::vec3 pos;
            switch(i) {
                case 0 : {
                    pos = glm::normalize(glm::vec3(b, a, 0.f)) * mult;
                    break;
                }
                case 1 : {
                    pos = glm::normalize(glm::vec3(0.f, b, a)) * mult;
                    break;
                }
                case 2 : {
                    pos = glm::normalize(glm::vec3(a, 0.f, b)) * mult;
                    break;
                }
            }

            const glm::vec3 normal = glm::normalize(pos);
            tempVertices.push_back({ pos, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    for (unsigned int j = 0u; j < 4u; ++j) {
        for (unsigned int i = 0u; i < 5u; ++i) {
            unsigned int ia, ib, ic;
            switch (j) {
                case 0u : {
                    ia = 0u;
                    ib = (11u + 3u * mul_2(i - div_4(i)) + mul_2(div_2(i)) + div_4(i)) % 12u;
                    ic = (5u + mul_8(div_4(i + 3u)) + 3u * mod_4(i + 3u) * div_4(i + 2u) + mul_2(mod_2(div_2(i))) + div_4(i + 2u)) % 12u;
                    break;
                }
                case 1u : {
                    ia = (1u + mul_2(2u + 3u * mod_4(i + 3u) * div_4(i + 2)) * div_4(i + 3u) + 3u * ((i / 3u) + div_4(i)) + mul_2(i / 3u)) % 12u;
                    ib = (5u + 3u * mul_2(i) + 3u * ((i / 3u) + 1u) * div_4(i + 2u) + mul_2(div_4(i + 2u))) % 12u;
                    ic = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3)) * (div_4(i + 2u))) % 12u;
                    break;
                }
                case 2u : {
                    ia = 3u;
                    ib = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3)) * div_4(i + 2u)) % 12u;
                    ic = (4u + mul_2(5u + i * div_4(i + 2u)) * div_4(i + 3u) - div_4(i)) % 12u;
                    break;
                }
                case 3u : {
                    ia = (4u + mul_2(5u + i * div_4(i + 2u)) * div_4(i + 3u) - div_4(i)) % 12u;
                    ib = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3u)) * div_4(i + 2u)) % 12u;
                    ic = (5u + 3u * mul_2(i) + 3u * ((i / 3u) + 1u) * div_4(i + 2u) + mul_2(div_4(i + 2u))) % 12u;
                    break;
                }
            }

            tempIndices.push_back(ia);
            tempIndices.push_back(ib);
            tempIndices.push_back(ic);
        }
    }

    glm::vec3 tangent;
    if (!useFlatShading) {
        _vertices.insert(_vertices.end(), tempVertices.begin(), tempVertices.end());

        const size_t tempIndSize = tempIndices.size();
        for (size_t i = 0ull; i < tempIndSize; i += 3ull) {
            const unsigned int ia = tempIndices[i];
            const unsigned int ib = tempIndices[i + 1ull];
            const unsigned int ic = tempIndices[i + 2ull];

            _indices.push_back(ia);
            _indices.push_back(ib);
            _indices.push_back(ic);

            if (!hasSubdivisions && _shapeConfig.genTangents) {
                tangent = _calcTangent(ia, ib, ic);

                _vertices[ia].Tangent += tangent;
                _vertices[ib].Tangent += tangent;
                _vertices[ic].Tangent += tangent;
            }
        }

        if (!hasSubdivisions && _shapeConfig.genTangents) {
            std::vector<unsigned int> trisNum(_vertices.size(), 5);
            _normalizeTangentsAndGenerateBitangents(trisNum, 0ull, _vertices.size());
        }
    }
    else {
        const size_t tempIndSize = tempIndices.size();
        for (size_t i = 0ull; i < tempIndSize; i += 3ull) {
            const size_t first = i;
            const size_t second = i + 1ull;
            const size_t third = i + 2ull;

            unsigned int ia = tempIndices[first];
            unsigned int ib = tempIndices[second];
            unsigned int ic = tempIndices[third];

            glm::vec3 normal = glm::normalize(glm::cross(tempVertices[ib].Normal - tempVertices[ia].Normal, tempVertices[ic].Normal - tempVertices[ia].Normal));

            _indices.push_back((unsigned int)first);
            _vertices.push_back({ tempVertices[ia].Position, tempVertices[ia].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            _indices.push_back((unsigned int)second);
            _vertices.push_back({ tempVertices[ib].Position, tempVertices[ib].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            _indices.push_back((unsigned int)third);
            _vertices.push_back({ tempVertices[ic].Position, tempVertices[ic].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            if (!hasSubdivisions && _shapeConfig.genTangents) {
                tangent = _calcTangent((unsigned int)first, (unsigned int)second, (unsigned int)third);

                _defineTangentBitangentFlatShading(tangent, first);
                _defineTangentBitangentFlatShading(tangent, second);
                _defineTangentBitangentFlatShading(tangent, third);
            }
        }
    }
}

void IcoSphere::_generate(const unsigned int subdivisions, const ValuesRange range, const bool useFlatShading)
{
    const float mult = (range == ValuesRange::HALF_TO_HALF) ? .5f : 1.f;
    _generateIcoSahedron(mult, useFlatShading, subdivisions != 0u);

    for (unsigned int i = 0u; i < subdivisions; ++i) {
        std::vector<unsigned int> newIndices;
        const size_t indSize = _indices.size();
        for (size_t j = 0ull; j < indSize; j += 3ull) {
            const unsigned int a = _indices[j];
            const unsigned int b = _indices[j + 1ull];
            const unsigned int c = _indices[j + 2ull];
            const unsigned int ab = useFlatShading ? _getMiddlePointFlatShading(a, b, mult) : _getMiddlePoint(a, b, mult);
            const unsigned int bc = useFlatShading ? _getMiddlePointFlatShading(b, c, mult) : _getMiddlePoint(b, c, mult);
            const unsigned int ca = useFlatShading ? _getMiddlePointFlatShading(c, a, mult) : _getMiddlePoint(c, a, mult);

            newIndices.push_back(a);
            newIndices.push_back(ab);
            newIndices.push_back(ca);

            newIndices.push_back(b);
            newIndices.push_back(bc);
            newIndices.push_back(ab);

            newIndices.push_back(c);
            newIndices.push_back(ca);
            newIndices.push_back(bc);

            newIndices.push_back(ab);
            newIndices.push_back(bc);
            newIndices.push_back(ca);
        }

        _indices.clear();
        _indices.insert(_indices.end(), newIndices.begin(), newIndices.end());
    }

    glm::vec3 tangent;
    const size_t indSize = _indices.size();
    if (!useFlatShading && _shapeConfig.genTangents) {
        for (size_t i = 0ull; i < indSize; i += 3ull) {
            const unsigned int ia = _indices[i];
            const unsigned int ib = _indices[i + 1ull];
            const unsigned int ic = _indices[i + 2ull];

            tangent = _calcTangent(ia, ib, ic);

            _vertices[ia].Tangent += tangent;
            _vertices[ib].Tangent += tangent;
            _vertices[ic].Tangent += tangent;
        }

        std::vector<unsigned int> trisNum(_vertices.size(), 5);
        _normalizeTangentsAndGenerateBitangents(trisNum, 0ull, _vertices.size());
    }
    else {
        for (size_t i = 0ull; i < indSize; i += 3ull) {
            const unsigned int ia = _indices[i];
            const unsigned int ib = _indices[i + 1ull];
            const unsigned int ic = _indices[i + 2ull];

            glm::vec3 normal = glm::normalize(glm::cross(glm::normalize(_vertices[ib].Position) - glm::normalize(_vertices[ia].Position), glm::normalize(_vertices[ic].Position) - glm::normalize(_vertices[ia].Position)));

            _vertices[ia].Normal = normal;
            _vertices[ib].Normal = normal;
            _vertices[ic].Normal = normal;

            if (_shapeConfig.genTangents) {
                tangent = _calcTangent(ia, ib, ic);

                _defineTangentBitangentFlatShading(tangent, ia);
                _defineTangentBitangentFlatShading(tangent, ib);
                _defineTangentBitangentFlatShading(tangent, ic);
            }
        }
    }
}

unsigned int IcoSphere::_getMiddlePoint(const unsigned int p1, const unsigned int p2, const float mult)
{
    const uint64_t key = (static_cast<uint64_t>(std::min(p1, p2)) << 32) | std::max(p1, p2);
    if (auto it = _middlePointCache.find(key); it != _middlePointCache.end()) {
        return it->second;
    }

    const glm::vec3 middle = glm::normalize((_vertices[p1].Position + _vertices[p2].Position) * 0.5f) * mult;
    const glm::vec3 normal = glm::normalize(middle);
    _middlePointCache[key] = (unsigned int)_vertices.size();
    _vertices.push_back({ middle, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return _middlePointCache[key];
}

unsigned int IcoSphere::_getMiddlePointFlatShading(const unsigned int p1, const unsigned int p2, const float mult)
{
    const glm::vec3 middle = glm::normalize((_vertices[p1].Position + _vertices[p2].Position) * 0.5f) * mult;
    const glm::vec3 normal = glm::normalize(middle);
    const unsigned int index = (unsigned int)_vertices.size();
    _vertices.push_back({ middle, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return index;
}

glm::vec2 IcoSphere::_getTexCoord(const glm::vec3 normal) const
{
    const float theta = (atan2f(normal.x, normal.z) * .5f) * (float)M_1_PI + .5f;
    const float phi = asinf(-normal.y) * (float)M_1_PI + .5f;
    return glm::vec2(theta, phi);
}

void IcoSphere::_defineTangentBitangentFlatShading(const glm::vec3 tangent, const size_t index)
{
    _vertices[index].Tangent = tangent;
    _normalizeTangentAndGenerateBitangent(index);
}

IcoSphere::IcoSphere(const ShapeConfig& config, const unsigned int subdivisions, const IcoSphereShading shading, const ValuesRange range)
{
    _shapeConfig = config;
    _vertices.clear();
    _indices.clear();
    _middlePointCache.clear();
    _generate(subdivisions, range, shading == IcoSphereShading::FLAT);
}

IcoSphere::~IcoSphere() {}

std::string IcoSphere::getClassName()
{
    return "IcoSphere";
}

std::string IcoSphere::getObjectClassName() const
{
    return IcoSphere::getClassName();
}
