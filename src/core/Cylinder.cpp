#include "pch.hpp"
#include "Cylinder.hpp"
#include "BitMathOperators.hpp"
#include "Shape.hpp"
#include "Config.hpp"
#include "Constants.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

using namespace config;

void Cylinder::_generateCircle(const unsigned int segments, const float y, const CylinderCullFace cullFace, const ValuesRange range)
{
    const Config& config = get_config();
    std::vector<unsigned int> trisNum;

    const float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    const float angleXZDiff = 2.f * (float)M_PI / (float)segments;

    // CIRCLE TOP
    // VERTICES AND TEX COORDS
    const size_t start = _vertices.size();
    float angleXZ = 0.f;
    for (unsigned int j = 0u; j < segments; ++j) {
        const float z = cosf(angleXZ);
        const float x = sinf(angleXZ);
        _vertices.push_back({ { x * mult, y, z * mult }, { .5f + x * .5f, .5f + z * .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
        if (config.genTangents) trisNum.push_back(2u);
        angleXZ += angleXZDiff;
    }
    _vertices.push_back({ { 0.f, y, 0.f }, { .5f, .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
    if (config.genTangents) trisNum.push_back(segments);

    // INDICES
    const size_t vertSize = _vertices.size();
    glm::vec3 tangent;
    for (size_t i = start; i < vertSize - 1ull; ++i) {
        const size_t right = i + 2ull == vertSize ? start : i + 1ull;

        _indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? i : right));
        _indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? right : i));
        _indices.push_back((unsigned int)vertSize - 1u);

        if (config.genTangents) {
            if (cullFace == CylinderCullFace::FRONT) {
                tangent = _calcTangent((unsigned int)i, (unsigned int)right, (unsigned int)vertSize - 1u);
            }
            else {
                tangent = _calcTangent((unsigned int)right, (unsigned int)i, (unsigned int)vertSize - 1u);
            }

            _vertices[i].Tangent += tangent;
            _vertices[right].Tangent += tangent;
            _vertices[vertSize - 1ull].Tangent += tangent;
        }
    }

    if (config.genTangents) _normalizeTangentsAndGenerateBitangents(trisNum, start, _vertices.size());

    trisNum.clear();
}

void Cylinder::_generate(const unsigned int horizontalSegments, const unsigned int verticalSegments, const ValuesRange range, const bool useFlatShading)
{
    const Config& config = get_config();
    const float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    const float h = 2.f * mult;

    _generateCircle(verticalSegments, h * 0.5f, CylinderCullFace::FRONT, range);

    std::vector<unsigned int> trisNum;

    const float angleXZDiff = 2.f * (float)M_PI / (float)verticalSegments;
    const float hDiff = h / (float)horizontalSegments;

    const size_t start = _vertices.size();

    // VERTICES UP AND DOWN
    const unsigned int horiSegms = (useFlatShading ? mul_2(horizontalSegments) : horizontalSegments + 1u);
    for (unsigned int i = 0u; i < horiSegms; ++i) {
        const float yDiff = hDiff * (float)(i - (useFlatShading ? div_2(i) : 0u));
        const float y = h * 0.5f - yDiff;
        float angleXZ = 0.f;
        const unsigned int vertSegms = verticalSegments + (useFlatShading ? 0u : 1u);
        for (unsigned int j = 0u; j < vertSegms; ++j) {
            if (useFlatShading) {
                const float x_n = (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * 0.5f;
                const float z_n = (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * 0.5f;

                const glm::vec3 norm = glm::normalize(glm::vec3(x_n, 0.f, z_n));

                for (unsigned int f = 0u; f < 2u; ++f) {
                    const float angleXZF = angleXZ + angleXZDiff * f;
                    const float z = cosf(angleXZF) * mult;
                    const float x = sinf(angleXZF) * mult;

                    _vertices.push_back({ { x, y, z }, { (float)f, yDiff / h }, norm, glm::vec3(0.f), glm::vec3(0.f) });
                    if (config.genTangents) trisNum.push_back(1u + mod_2(i + f));
                }
            }
            else {
                const float x_n = sinf(angleXZ);
                const float z_n = cosf(angleXZ);

                _vertices.push_back({ { x_n * mult, y, z_n * mult }, { (float)angleXZ * 0.5f * M_1_PI, yDiff / h }, glm::normalize(glm::vec3(x_n, 0.f, z_n)), glm::vec3(0.f), glm::vec3(0.f) });

                if (config.genTangents) {
                    const unsigned int count =
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
            }
            angleXZ += angleXZDiff;
        }
    }

    // INDICES
    glm::vec3 tangent;
    for (unsigned int i = 0u; i < horizontalSegments; ++i) {
        for (unsigned int j = 0u; j < verticalSegments; ++j) {
            const size_t f = (useFlatShading ? 1ull : 0ull);
            const size_t m = f + 1ull;

            const size_t flatDiffrence = m * (verticalSegments - 1ull) * (size_t)i * f;

            size_t left = start + ((size_t)j + (size_t)i * ((size_t)verticalSegments + 1ull)) * m + flatDiffrence;
            size_t dt = start + ((size_t)j + (size_t)(i + 1ull) * (size_t)verticalSegments + (size_t)i) * m + mod_2(m) + flatDiffrence;
            size_t right = start + ((size_t)j + (size_t)i * ((size_t)verticalSegments + 1ull)) * m + 1ull + flatDiffrence;

            _indices.push_back((unsigned int)left);
            _indices.push_back((unsigned int)dt);
            _indices.push_back((unsigned int)right);

            if (config.genTangents) {
                tangent = _calcTangent((unsigned int)left, (unsigned int)dt, (unsigned int)right);

                _vertices[left].Tangent += tangent;
                _vertices[dt].Tangent += tangent;
                _vertices[right].Tangent += tangent;
            }

            std::swap(dt, right);
            left = right;
            right += 1ull;

            _indices.push_back((unsigned int)dt);
            _indices.push_back((unsigned int)left);
            _indices.push_back((unsigned int)right);

            if (config.genTangents) {
                tangent = _calcTangent((unsigned int)dt, (unsigned int)left, (unsigned int)right);

                _vertices[dt].Tangent += tangent;
                _vertices[left].Tangent += tangent;
                _vertices[right].Tangent += tangent;
            }
        }
    }

    if (config.genTangents) _normalizeTangentsAndGenerateBitangents(trisNum, start, _vertices.size());

    trisNum.clear();

    _generateCircle(verticalSegments, -h * 0.5f, CylinderCullFace::BACK, range);
}

Cylinder::Cylinder(const unsigned int horizontalSegments, const unsigned int verticalSegments, const CylinderShading shading, const ValuesRange range)
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