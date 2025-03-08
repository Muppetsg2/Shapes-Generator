// PRECOMPILED HEADER
#include "pch.h"
#include "IcoSphere.h"

void IcoSphere::generateIcoSahedron(float mult, bool useFlatShading, bool hasSubdivisions)
{
    std::vector<Vertex> tempVertices;
    std::vector<unsigned int> tempIndices;

    float t = (1.f + sqrtf(5.f)) * .5f;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            float a = t * (float)(1 - 2 * (int)(j / 2));
            float b = (float)(1 - 2 * ((j + 1) % 2));
            glm::vec3 pos;
            switch(i) {
                case 0: {
                    pos = glm::normalize(glm::vec3(b, a, 0.f)) * mult;
                    break;
                }
                case 1: {
                    pos = glm::normalize(glm::vec3(0.f, b, a)) * mult;
                    break;
                }
                case 2: {
                    pos = glm::normalize(glm::vec3(a, 0.f, b)) * mult;
                    break;
                }
            }

            glm::vec3 normal = glm::normalize(pos);
            tempVertices.push_back({ pos, getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            unsigned int ia, ib, ic;
            switch (i) {
                case 0: {
                    ia = 0u;
                    ib = (unsigned int)((11 + 6 * (j - ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2)) + 2 * (int)(j / 2) + (int)(j / 4)) % 12);
                    ic = (unsigned int)((5 + 8 * ((((((j + 1) % 3) % 2) - (int)((j % 4) / 3)) + 1) % 2) + 3 * ((j - 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) + 2 * ((int)(j / 2) % 2) + ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 1: {
                    ia = (unsigned int)((1 + 2 * (2 + 3 * ((j - 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2))) * (((((j + 1) % 3) % 2) - (int)((j % 4) / 3) + 1) % 2) + 3 * ((j - 2) * ((int)((j % 4) / 3) + ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2))) + 2 * ((int)((j % 4) / 3) + ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2))) % 12);
                    ib = (unsigned int)((5 + 6 * j + 3 * (((int)(j / 3) + 1) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) + 2 * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    ic = (unsigned int)((9 + 7 * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) + 2 * (2 * j + 1 * ((int)((j % 4) / 3) + ((int)((j + ((int)((j + 1) / 4) % 2)) / 2) % 2))) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 2: {
                    ia = 3u;
                    ib = (unsigned int)((9 + 7 * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) + 2 * (2 * j + 1 * ((int)((j % 4) / 3) + ((int)((j + ((int)((j + 1) / 4) % 2)) / 2) % 2))) * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) % 12);
                    ic = (unsigned int)((4 + 2 * (5 + j * ((int)((j + 2 * ((int)(j / 4) % 2)) / 2) % 2)) * (((((j + 1) % 3) % 2 - (int)((j % 4) / 3)) + 1) % 2) - ((int)((j + 2 * ((int)((j + 2) / 4) % 2)) / 2) % 2)) % 12);
                    break;
                }
                case 3: {
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

    if (!useFlatShading) {
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());

        for (size_t i = 0; i < tempIndices.size(); i += 3) {
            unsigned int ia = tempIndices[i];
            unsigned int ib = tempIndices[i + 1];
            unsigned int ic = tempIndices[i + 2];

            indices.push_back(ia);
            indices.push_back(ib);
            indices.push_back(ic);

            if (!hasSubdivisions) {
                std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(ia, ib, ic);

                vertices[ia].Tangent += TB.first;
                vertices[ia].Bitangent += TB.second;

                vertices[ib].Tangent += TB.first;
                vertices[ib].Bitangent += TB.second;

                vertices[ic].Tangent += TB.first;
                vertices[ic].Bitangent += TB.second;
            }
        }

        if (!hasSubdivisions) {
            for (size_t i = 0; i < vertices.size(); ++i) {
                vertices[i].Tangent *= .2f;

                if (glm::length(vertices[i].Tangent) != 0.f) {
                    vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
                }

                vertices[i].Bitangent *= .2f;

                if (glm::length(vertices[i].Bitangent) != 0.f) {
                    vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
                }
            }
        }
    }
    else {
        for (size_t i = 0; i < tempIndices.size(); i += 3ull) {
            unsigned int ia = tempIndices[i];
            unsigned int ib = tempIndices[i + 1ull];
            unsigned int ic = tempIndices[i + 2ull];

            glm::vec3 normal = glm::normalize(glm::cross(tempVertices[ib].Normal - tempVertices[ia].Normal, tempVertices[ic].Normal - tempVertices[ia].Normal));

            indices.push_back((unsigned int)i);
            vertices.push_back({ tempVertices[ia].Position, tempVertices[ia].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            indices.push_back((unsigned int)i + 1u);
            vertices.push_back({ tempVertices[ib].Position, tempVertices[ib].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            indices.push_back((unsigned int)i + 2u);
            vertices.push_back({ tempVertices[ic].Position, tempVertices[ic].TexCoord, normal, glm::vec3(0.f), glm::vec3(0.f) });

            if (!hasSubdivisions) {
                std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)i, (unsigned int)i + 1u, (unsigned int)i + 2u);

                defineTangentBitangentFlatShading(TB, i);
                defineTangentBitangentFlatShading(TB, i + 1ull);
                defineTangentBitangentFlatShading(TB, i + 2ull);
            }
        }
    }
}

void IcoSphere::generate(unsigned int subdivisions, ValuesRange range, bool useFlatShading)
{
    float mult = (range == ValuesRange::HALF_TO_HALF) ? .5f : 1.f;
    generateIcoSahedron(mult, useFlatShading, subdivisions != 0u);

    for (unsigned int i = 0; i < subdivisions; ++i) {
        std::vector<unsigned int> newIndices;
        for (size_t j = 0; j < indices.size(); j += 3) {
            unsigned int a = indices[j];
            unsigned int b = indices[j + 1];
            unsigned int c = indices[j + 2];
            unsigned int ab = useFlatShading ? getMiddlePointFlatShading(a, b, mult) : getMiddlePoint(a, b, mult);
            unsigned int bc = useFlatShading ? getMiddlePointFlatShading(b, c, mult) : getMiddlePoint(b, c, mult);
            unsigned int ca = useFlatShading ? getMiddlePointFlatShading(c, a, mult) : getMiddlePoint(c, a, mult);

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

        indices.clear();
        indices.insert(indices.end(), newIndices.begin(), newIndices.end());
    }

    if (!useFlatShading) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            unsigned int ia = indices[i];
            unsigned int ib = indices[i + 1];
            unsigned int ic = indices[i + 2];

            std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(ia, ib, ic);

            vertices[ia].Tangent += TB.first;
            vertices[ia].Bitangent += TB.second;

            vertices[ib].Tangent += TB.first;
            vertices[ib].Bitangent += TB.second;

            vertices[ic].Tangent += TB.first;
            vertices[ic].Bitangent += TB.second;
        }

        for (size_t i = 0; i < vertices.size(); ++i) {
            vertices[i].Tangent *= .2f;

            if (glm::length(vertices[i].Tangent) != 0.f) {
                vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
            }

            vertices[i].Bitangent *= .2f;

            if (glm::length(vertices[i].Bitangent) != 0.f) {
                vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
            }
        }
    }
    else {
        for (size_t i = 0; i < indices.size(); i += 3) {
            unsigned int ia = indices[i];
            unsigned int ib = indices[i + 1];
            unsigned int ic = indices[i + 2];

            glm::vec3 normal = glm::normalize(glm::cross(glm::normalize(vertices[ib].Position) - glm::normalize(vertices[ia].Position), glm::normalize(vertices[ic].Position) - glm::normalize(vertices[ia].Position)));

            vertices[ia].Normal = normal;
            vertices[ib].Normal = normal;
            vertices[ic].Normal = normal;

            std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(ia, ib, ic);

            defineTangentBitangentFlatShading(TB, ia);
            defineTangentBitangentFlatShading(TB, ib);
            defineTangentBitangentFlatShading(TB, ic);
        }
    }
}

unsigned int IcoSphere::getMiddlePoint(unsigned int p1, unsigned int p2, float mult)
{
    uint64_t key = (static_cast<uint64_t>(std::min(p1, p2)) << 32) | std::max(p1, p2);
    if (auto it = middlePointCache.find(key); it != middlePointCache.end()) {
        return it->second;
    }

    glm::vec3 middle = glm::normalize((vertices[p1].Position + vertices[p2].Position) * 0.5f) * mult;
    glm::vec3 normal = glm::normalize(middle);
    middlePointCache[key] = (unsigned int)vertices.size();
    vertices.push_back({ middle, getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return middlePointCache[key];
}

unsigned int IcoSphere::getMiddlePointFlatShading(unsigned int p1, unsigned int p2, float mult)
{
    glm::vec3 middle = glm::normalize((vertices[p1].Position + vertices[p2].Position) * 0.5f) * mult;
    glm::vec3 normal = glm::normalize(middle);
    unsigned int index = (unsigned int)vertices.size();
    vertices.push_back({ middle, getTexCoord(normal), normal, glm::vec3(0.f), glm::vec3(0.f) });
    return index;
}

glm::vec2 IcoSphere::getTexCoord(glm::vec3 normal)
{
    float theta = (atan2f(normal.x, normal.z) * .5f) / glm::pi<float>() + .5f;
    float phi = asinf(-normal.y) / glm::pi<float>() + .5f;
    return glm::vec2(theta, phi);
}

void IcoSphere::defineTangentBitangentFlatShading(std::pair<glm::vec3, glm::vec3> TB, size_t index)
{
    vertices[index].Tangent = TB.first;

    if (glm::length(vertices[index].Tangent) != 0.f) {
        vertices[index].Tangent = glm::normalize(vertices[index].Tangent);
    }

    vertices[index].Bitangent = TB.second;

    if (glm::length(vertices[index].Bitangent) != 0.f) {
        vertices[index].Bitangent = glm::normalize(vertices[index].Bitangent);
    }
}

IcoSphere::IcoSphere(unsigned int subdivisions, IcoSphereShading shading, ValuesRange range)
{
    vertices.clear();
    indices.clear();
    middlePointCache.clear();
    generate(subdivisions, range, shading == IcoSphereShading::FLAT);
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
