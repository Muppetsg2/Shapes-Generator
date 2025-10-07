#include "IcoSphere.hpp"
#include "BitMathOperators.hpp"

void IcoSphere::_generateIcoSahedron(const float mult, const bool useFlatShading, const bool hasSubdivisions)
{
    const Config& config = get_config();

    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;

    constexpr float t = .5f + (float)M_SQRT5 * .5f;

    for (unsigned int i = 0u; i < 3u; ++i) {
        for (unsigned int j = 0u; j < 4u; ++j) {
            const float a = t * (float)(1 - mul_2(div_2(j)));
            const float b = (float)(1 - mul_2(mod_2(j + 1)));
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

    for (unsigned int i = 0u; i < 4u; ++i) {
        for (unsigned int j = 0u; j < 5u; ++j) {
            unsigned int ia, ib, ic;
            switch (i) {
                case 0u : {
                    ia = 0u;
                    ib = (11u + 3u * mul_2(i - div_4(i)) + mul_2(div_2(i)) + div_4(i)) % 12u;
                    ic = (5u + mul_8(div_4(i + 3u)) + 3u * mod_4(i) * div_4(i + 2u) + mul_2(mod_2(div_2(i))) + div_4(i + 2u)) % 12u;
                    break;
                }
                case 1u : {
                    ia = (1u + mul_2(2u + 3u * mod_4(i + 3u) * div_4(i + 2)) * div_4(i + 3u) + 3u * ((i / 3u) + div_4(i)) + mul_2(i / 3u)) % 12u;
                    ib = (5u + 3u * mul_2(i) + 3u * (((i / 3u) + 1u) * div_4(i + 2u)) + mul_2(div_4(i + 2u))) % 12u;
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

    std::pair<glm::vec3, glm::vec3> TB;
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

            if (!hasSubdivisions && config.genTangents) {
                TB = _calcTangentBitangent(ia, ib, ic);

                _vertices[ia].Tangent += TB.first;
                _vertices[ia].Bitangent += TB.second;

                _vertices[ib].Tangent += TB.first;
                _vertices[ib].Bitangent += TB.second;

                _vertices[ic].Tangent += TB.first;
                _vertices[ic].Bitangent += TB.second;
            }
        }

        if (!hasSubdivisions && config.genTangents) {
            const size_t vertSize = _vertices.size();
            for (size_t i = 0ull; i < vertSize; ++i) {
                _vertices[i].Tangent *= .2f;

                if (fabsf(glm::length(_vertices[i].Tangent)) >= EPSILON) {
                    _vertices[i].Tangent = glm::normalize(_vertices[i].Tangent);
                }

                _vertices[i].Bitangent *= .2f;

                if (fabsf(glm::length(_vertices[i].Bitangent)) >= EPSILON) {
                    _vertices[i].Bitangent = glm::normalize(_vertices[i].Bitangent);
                }
            }
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

            if (!hasSubdivisions && config.genTangents) {
                TB = _calcTangentBitangent((unsigned int)first, (unsigned int)second, (unsigned int)third);

                _defineTangentBitangentFlatShading(TB, first);
                _defineTangentBitangentFlatShading(TB, second);
                _defineTangentBitangentFlatShading(TB, third);
            }
        }
    }
}

void IcoSphere::_generate(const unsigned int subdivisions, const ValuesRange range, const bool useFlatShading)
{
    const Config& config = get_config();

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

    std::pair<glm::vec3, glm::vec3> TB;
    const size_t indSize = _indices.size();
    if (!useFlatShading && config.genTangents) {
        for (size_t i = 0ull; i < indSize; i += 3ull) {
            const unsigned int ia = _indices[i];
            const unsigned int ib = _indices[i + 1ull];
            const unsigned int ic = _indices[i + 2ull];

            TB = _calcTangentBitangent(ia, ib, ic);

            _vertices[ia].Tangent += TB.first;
            _vertices[ia].Bitangent += TB.second;

            _vertices[ib].Tangent += TB.first;
            _vertices[ib].Bitangent += TB.second;

            _vertices[ic].Tangent += TB.first;
            _vertices[ic].Bitangent += TB.second;
        }

        const size_t vertSize = _vertices.size();
        for (size_t i = 0ull; i < vertSize; ++i) {
            _vertices[i].Tangent *= .2f;

            if (fabsf(glm::length(_vertices[i].Tangent)) >= EPSILON) {
                _vertices[i].Tangent = glm::normalize(_vertices[i].Tangent);
            }

            _vertices[i].Bitangent *= .2f;

            if (fabsf(glm::length(_vertices[i].Bitangent)) >= EPSILON) {
                _vertices[i].Bitangent = glm::normalize(_vertices[i].Bitangent);
            }
        }
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

            if (config.genTangents) {
                TB = _calcTangentBitangent(ia, ib, ic);

                _defineTangentBitangentFlatShading(TB, ia);
                _defineTangentBitangentFlatShading(TB, ib);
                _defineTangentBitangentFlatShading(TB, ic);
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

void IcoSphere::_defineTangentBitangentFlatShading(const std::pair<glm::vec3, glm::vec3> TB, const size_t index)
{
    _vertices[index].Tangent = TB.first;

    if (fabsf(glm::length(_vertices[index].Tangent)) >= EPSILON) {
        _vertices[index].Tangent = glm::normalize(_vertices[index].Tangent);
    }

    _vertices[index].Bitangent = TB.second;

    if (fabsf(glm::length(_vertices[index].Bitangent)) >= EPSILON) {
        _vertices[index].Bitangent = glm::normalize(_vertices[index].Bitangent);
    }
}

IcoSphere::IcoSphere(const unsigned int subdivisions, const IcoSphereShading shading, const ValuesRange range)
{
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
