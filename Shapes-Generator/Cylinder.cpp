#include "pch.hpp"
#include "Cylinder.hpp"

void Cylinder::_generateCircle(unsigned int segments, float y, CylinderCullFace cullFace, ValuesRange range)
{
    std::vector<unsigned int> trisNum;

    float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    float angleXZDiff = 2.f * (float)M_PI / (float)segments;

    // CIRCLE TOP
    // VERTICES AND TEX COORDS
    size_t start = _vertices.size();
    float angleXZ = 0.f;
    for (unsigned int j = 0u; j < segments; ++j) {
        float z = cosf(angleXZ);
        float x = sinf(angleXZ);
        _vertices.push_back({ { x * mult, y, z * mult }, { .5f + x * .5f, .5f + z * .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
        trisNum.push_back(2u);
        angleXZ += angleXZDiff;
    }
    _vertices.push_back({ { 0.f, y, 0.f }, { .5f, .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
    trisNum.push_back(segments);

    // INDICES
    const size_t vertSize = _vertices.size();
    std::pair<glm::vec3, glm::vec3> TB;
    for (size_t i = start; i < vertSize - 1ull; ++i) {
        size_t right = i + 2ull == vertSize ? start : i + 1ull;

        _indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? i : right));
        _indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? right : i));
        _indices.push_back((unsigned int)vertSize - 1u);

        if (cullFace == CylinderCullFace::FRONT) {
            TB = _calcTangentBitangent((unsigned int)i, (unsigned int)right, (unsigned int)vertSize - 1u);
        }
        else {
            TB = _calcTangentBitangent((unsigned int)right, (unsigned int)i, (unsigned int)vertSize - 1u);
        }

        _vertices[i].Tangent += TB.first;
        _vertices[i].Bitangent += TB.second;

        _vertices[right].Tangent += TB.first;
        _vertices[right].Bitangent += TB.second;

        _vertices[vertSize - 1ull].Tangent += TB.first;
        _vertices[vertSize - 1ull].Bitangent += TB.second;
    }

    _normalizeTangents(trisNum, start, _vertices.size());

    trisNum.clear();
}

void Cylinder::_generate(unsigned int horizontalSegments, unsigned int verticalSegments, ValuesRange range, bool useFlatShading)
{
    float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    float h = 2.f * mult;

    _generateCircle(verticalSegments, h * 0.5f, CylinderCullFace::FRONT, range);

    std::vector<unsigned int> trisNum;

    float angleXZDiff = 2.f * (float)M_PI / (float)verticalSegments;
    float hDiff = h / (float)horizontalSegments;

    size_t start = _vertices.size();

    // VERTICES UP AND DOWN
    unsigned int horiSegms = (useFlatShading ? horizontalSegments * 2u : horizontalSegments + 1u);
    for (unsigned int i = 0u; i < horiSegms; ++i) {
        float yDiff = hDiff * (float)(i - (useFlatShading ? i / 2u : 0u));
        float y = h * 0.5f - yDiff;
        float angleXZ = 0.f;
        unsigned int vertSegms = verticalSegments + (useFlatShading ? 0u : 1u);
        for (unsigned int j = 0u; j < vertSegms; ++j) {
            if (useFlatShading) {
                float x_n = (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * 0.5f;
                float z_n = (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * 0.5f;

                glm::vec3 norm = glm::normalize(glm::vec3(x_n, 0.f, z_n));

                for (unsigned int f = 0u; f < 2u; ++f) {
                    float angleXZF = angleXZ + angleXZDiff * f;
                    float z = cosf(angleXZF) * mult;
                    float x = sinf(angleXZF) * mult;

                    _vertices.push_back({ { x, y, z }, { (float)f, yDiff / h }, norm, glm::vec3(0.f), glm::vec3(0.f) });
                    trisNum.push_back(1u + (i + f) % 2u);
                }
            }
            else {
                float x_n = sinf(angleXZ);
                float z_n = cosf(angleXZ);

                _vertices.push_back({ { x_n * mult, y, z_n * mult }, { (float)angleXZ * 0.5f * M_1_PI, yDiff / h }, glm::normalize(glm::vec3(x_n, 0.f, z_n)), glm::vec3(0.f), glm::vec3(0.f) });

                unsigned int count =
                    (j == 0u || j == verticalSegments
                        ? (i == 0u || i == horizontalSegments
                            ? 1u
                            : 3u)
                        : (i == 0u || i == horizontalSegments
                            ? 3u
                            : 6u))
                    + ((i == 0u && j == verticalSegments) ||
                        (i == horizontalSegments && j == 0u)
                        ? 1u
                        : 0u);
                trisNum.push_back(count);
            }
            angleXZ += angleXZDiff;
        }
    }

    // INDICES
    std::pair<glm::vec3, glm::vec3> TB;
    for (unsigned int i = 0u; i < horizontalSegments; ++i) {
        for (unsigned int j = 0u; j < verticalSegments; ++j) {
            size_t f = (useFlatShading ? 1ull : 0ull);
            size_t m = f + 1ull;

            size_t flatDiffrence = m * (verticalSegments - 1ull) * (size_t)i * f;

            size_t left = start + ((size_t)j + (size_t)i * ((size_t)verticalSegments + 1ull)) * m + flatDiffrence;
            size_t dt = start + ((size_t)j + (size_t)(i + 1ull) * (size_t)verticalSegments + (size_t)i) * m + m % 2ull + flatDiffrence;
            size_t right = start + ((size_t)j + (size_t)i * ((size_t)verticalSegments + 1ull)) * m + 1ull + flatDiffrence;

            _indices.push_back((unsigned int)left);
            _indices.push_back((unsigned int)dt);
            _indices.push_back((unsigned int)right);

            TB = _calcTangentBitangent((unsigned int)left, (unsigned int)dt, (unsigned int)right);

            _vertices[left].Tangent += TB.first;
            _vertices[left].Bitangent += TB.second;

            _vertices[dt].Tangent += TB.first;
            _vertices[dt].Bitangent += TB.second;

            _vertices[right].Tangent += TB.first;
            _vertices[right].Bitangent += TB.second;

            std::swap(dt, right);
            left = right;
            right += 1ull;

            _indices.push_back((unsigned int)dt);
            _indices.push_back((unsigned int)left);
            _indices.push_back((unsigned int)right);

            TB = _calcTangentBitangent((unsigned int)dt, (unsigned int)left, (unsigned int)right);

            _vertices[dt].Tangent += TB.first;
            _vertices[dt].Bitangent += TB.second;

            _vertices[left].Tangent += TB.first;
            _vertices[left].Bitangent += TB.second;

            _vertices[right].Tangent += TB.first;
            _vertices[right].Bitangent += TB.second;
        }
    }

    _normalizeTangents(trisNum, start, _vertices.size());

    trisNum.clear();

    _generateCircle(verticalSegments, -h * 0.5f, CylinderCullFace::BACK, range);
}

Cylinder::Cylinder(unsigned int horizontalSegments, unsigned int verticalSegments, CylinderShading shading, ValuesRange range)
{
    _vertices.clear();
    _indices.clear();
    _generate(std::max(1u, horizontalSegments), std::max(3u, verticalSegments), range, shading == CylinderShading::FLAT);
}

Cylinder::~Cylinder() {}

std::string Cylinder::getClassName()
{
    return "Cylinder";
}

std::string Cylinder::getObjectClassName() const
{
    return Cylinder::getClassName();
}