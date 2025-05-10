#include "IcoSphere.hpp"

void IcoSphere::_generateIcoSahedron(float mult, bool useFlatShading, bool hasSubdivisions)
{
    const Config& config = get_config();

    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;

    float t = .5f + (float)M_SQRT5 * .5f;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            float a = t * (float)(1 - 2 * (int)(j / 2));
            float b = (float)(1 - 2 * ((j + 1) % 2));
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

            glm::vec3 normal = glm::normalize(pos);
            tempVertices.push_back({ pos, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            unsigned int ia, ib, ic;
            switch (i) {
                case 0 : {
                    ia = 0u;
                    ib = (unsigned int)((11 + 6 * (j - ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2)) + 2 * (int)(j / 2) + (int)(j / 4)) % 12);
                    ic = (unsigned int)((5 + 8 * ((((((j + 1) % 3) % 2) - (int)((j % 4) / 3)) + 1) % 2) + 3 * ((j - 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) + 2 * ((int)(j / 2) % 2) + ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 1 : {
                    ia = (unsigned int)((1 + 2 * (2 + 3 * ((j - 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2))) * (((((j + 1) % 3) % 2) - (int)((j % 4) / 3) + 1) % 2) + 3 * ((j - 2) * ((int)((j % 4) / 3) + ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2))) + 2 * ((int)((j % 4) / 3) + ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2))) % 12);
                    ib = (unsigned int)((5 + 6 * j + 3 * (((int)(j / 3) + 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) + 2 * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    ic = (unsigned int)((9 + 7 * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) + 2 * (2 * j + 1 * ((int)((j % 4) / 3) + ((int)((j + ((int)((j + 1) / 4) % 2)) / 2) % 2))) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 2 : {
                    ia = 3u;
                    ib = (unsigned int)((9 + 7 * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) + 2 * (2 * j + 1 * ((int)((j % 4) / 3) + ((int)((j + ((int)((j + 1) / 4) % 2)) / 2) % 2))) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    ic = (unsigned int)((4 + 2 * (5 + j * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) - ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 3 : {
                    ia = (unsigned int)((4 + 2 * (5 + j * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) - ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2)) % 12);
                    ib = (unsigned int)((9 + 7 * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) + 2 * (2 * j + 1 * ((int)((j % 4) / 3) + ((int)((j + ((int)((j + 1) / 4) % 2)) / 2) % 2))) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    ic = (unsigned int)((5 + 6 * j + 3 * (((int)(j / 3) + 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) + 2 * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
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
            unsigned int ia = tempIndices[i];
            unsigned int ib = tempIndices[i + 1ull];
            unsigned int ic = tempIndices[i + 2ull];

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
            size_t first = i;
            size_t second = i + 1ull;
            size_t third = i + 2ull;

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

void IcoSphere::_generate(unsigned int subdivisions, ValuesRange range, bool useFlatShading)
{
    const Config& config = get_config();

    float mult = (range == ValuesRange::HALF_TO_HALF) ? .5f : 1.f;
    _generateIcoSahedron(mult, useFlatShading, subdivisions != 0u);

    for (unsigned int i = 0u; i < subdivisions; ++i) {
        std::vector<unsigned int> newIndices;
        const size_t indSize = _indices.size();
        for (size_t j = 0ull; j < indSize; j += 3ull) {
            unsigned int a = _indices[j];
            unsigned int b = _indices[j + 1ull];
            unsigned int c = _indices[j + 2ull];
            unsigned int ab = useFlatShading ? _getMiddlePointFlatShading(a, b, mult) : _getMiddlePoint(a, b, mult);
            unsigned int bc = useFlatShading ? _getMiddlePointFlatShading(b, c, mult) : _getMiddlePoint(b, c, mult);
            unsigned int ca = useFlatShading ? _getMiddlePointFlatShading(c, a, mult) : _getMiddlePoint(c, a, mult);

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
            unsigned int ia = _indices[i];
            unsigned int ib = _indices[i + 1ull];
            unsigned int ic = _indices[i + 2ull];

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
            unsigned int ia = _indices[i];
            unsigned int ib = _indices[i + 1ull];
            unsigned int ic = _indices[i + 2ull];

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

unsigned int IcoSphere::_getMiddlePoint(unsigned int p1, unsigned int p2, float mult)
{
    uint64_t key = (static_cast<uint64_t>(std::min(p1, p2)) << 32) | std::max(p1, p2);
    if (auto it = _middlePointCache.find(key); it != _middlePointCache.end()) {
        return it->second;
    }

    glm::vec3 middle = glm::normalize((_vertices[p1].Position + _vertices[p2].Position) * 0.5f) * mult;
    glm::vec3 normal = glm::normalize(middle);
    _middlePointCache[key] = (unsigned int)_vertices.size();
    _vertices.push_back({ middle, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return _middlePointCache[key];
}

unsigned int IcoSphere::_getMiddlePointFlatShading(unsigned int p1, unsigned int p2, float mult)
{
    glm::vec3 middle = glm::normalize((_vertices[p1].Position + _vertices[p2].Position) * 0.5f) * mult;
    glm::vec3 normal = glm::normalize(middle);
    unsigned int index = (unsigned int)_vertices.size();
    _vertices.push_back({ middle, _getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return index;
}

glm::vec2 IcoSphere::_getTexCoord(glm::vec3 normal)
{
    float theta = (atan2f(normal.x, normal.z) * .5f) * (float)M_1_PI + .5f;
    float phi = asinf(-normal.y) * (float)M_1_PI + .5f;
    return glm::vec2(theta, phi);
}

void IcoSphere::_defineTangentBitangentFlatShading(std::pair<glm::vec3, glm::vec3> TB, size_t index)
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

IcoSphere::IcoSphere(unsigned int subdivisions, IcoSphereShading shading, ValuesRange range)
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
