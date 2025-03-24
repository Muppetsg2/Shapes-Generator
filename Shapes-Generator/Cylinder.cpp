// PRECOMPILED HEADER
#include "pch.h"
#include "Cylinder.h"

void Cylinder::generateCircle(unsigned int segments, float y, CylinderCullFace cullFace, ValuesRange range)
{
    std::vector<unsigned int> trisNum;

    float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    float angleXZDiff = 360.f / (float)segments;

    // CIRCLE TOP
    // VERTICES AND TEX COORDS
    size_t start = vertices.size();

    float angleXZ = 0.f;
    for (unsigned int j = 0; j < segments; ++j) {
        float radiansXZ = glm::radians(angleXZ);
        float z = cosf(radiansXZ);
        float x = sinf(radiansXZ);
        vertices.push_back({ { x * mult, y, z * mult }, { .5f + x * .5f, .5f + z * .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
        trisNum.push_back(2);
        angleXZ += angleXZDiff;
    }
    vertices.push_back({ { 0.f, y, 0.f }, { .5f, .5f }, (cullFace == CylinderCullFace::FRONT ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
    trisNum.push_back(segments);

    // INDICES
    for (size_t i = start; i < vertices.size() - 1; ++i) {

        size_t right = i + 2 == vertices.size() ? start : i + 1;

        indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? i : right));
        indices.push_back((unsigned int)(cullFace == CylinderCullFace::FRONT ? right : i));
        indices.push_back((unsigned int)vertices.size() - 1u);

        std::pair<glm::vec3, glm::vec3> TB;

        if (cullFace == CylinderCullFace::FRONT) {
            TB = calcTangentBitangent((unsigned int)i, (unsigned int)right, (unsigned int)vertices.size() - 1u);
        }
        else {
            TB = calcTangentBitangent((unsigned int)right, (unsigned int)i, (unsigned int)vertices.size() - 1u);
        }

        vertices[i].Tangent += TB.first;
        vertices[i].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;

        vertices[vertices.size() - 1].Tangent += TB.first;
        vertices[vertices.size() - 1].Bitangent += TB.second;
    }

    for (size_t i = start; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i - start];

        if (glm::length(vertices[i].Tangent) != 0.f) {
            vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
        }

        vertices[i].Bitangent /= (float)trisNum[i - start];

        if (glm::length(vertices[i].Bitangent) != 0.f) {
            vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
        }
    }

    trisNum.clear();
}

void Cylinder::generate(unsigned int segments, ValuesRange range, bool useFlatShading)
{
    float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
    float h = 2.f * mult;

    generateCircle(segments, h * 0.5f, CylinderCullFace::FRONT, range);

    std::vector<unsigned int> trisNum;

    float angleXZDiff = 360.f / (float)segments;

    size_t start = vertices.size();

    // VERTICES UP AND DOWN
    for (int i = 0; i < 2; ++i) {
        float y = h * 0.5f - h * i;
        float angleXZ = 0.f;
        unsigned int segms = segments + (useFlatShading ? 0 : 1);
        for (unsigned int j = 0; j < segms; ++j) {

            float radiansXZ = glm::radians(angleXZ);

            if (useFlatShading) {
                float x_n = (sinf(radiansXZ) + sinf(radiansXZ + glm::radians(angleXZDiff))) * 0.5f;
                float z_n = (cosf(radiansXZ) + cosf(radiansXZ + glm::radians(angleXZDiff))) * 0.5f;

                for (int f = 0; f < 2; ++f) {
                    float radiansXZF = radiansXZ + glm::radians(angleXZDiff * f);
                    float z = cosf(radiansXZF) * mult;
                    float x = sinf(radiansXZF) * mult;
                    vertices.push_back({ { x, y, z }, { (float)f, (float)i }, glm::normalize(glm::vec3(x_n, 0.f, z_n)), glm::vec3(0.f), glm::vec3(0.f) });

                    trisNum.push_back(1 + (i + f) % 2);
                }
            }
            else {
                float x_n = sinf(radiansXZ);
                float z_n = cosf(radiansXZ);

                vertices.push_back({ { x_n * mult, y, z_n * mult }, { (float)angleXZ / 360.f, (float)i}, glm::normalize(glm::vec3(x_n, 0.f, z_n)), glm::vec3(0.f), glm::vec3(0.f) });

                unsigned int count = 1 + (j == 0 ? i : ((j == segments) ? (i + 1) % 2 : 2));
                trisNum.push_back(count);
            }

            angleXZ += angleXZDiff;
        }
    }

    // INDICES
    for (unsigned int i = 0; i < segments; ++i) {

        size_t m = (useFlatShading ? 2 : 1);

        size_t left = start + (size_t)i * m;
        size_t dt = start + ((size_t)i + (size_t)segments) * m + m % 2;
        size_t right = start + (size_t)i * m + 1;

        indices.push_back((unsigned int)left);
        indices.push_back((unsigned int)dt);
        indices.push_back((unsigned int)right);

        std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)left, (unsigned int)dt, (unsigned int)right);

        vertices[left].Tangent += TB.first;
        vertices[left].Bitangent += TB.second;

        vertices[dt].Tangent += TB.first;
        vertices[dt].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;

        dt = start + (size_t)i * m + 1;
        left = start + ((size_t)i + (size_t)segments) * m + m % 2;
        right = start + ((size_t)i + (size_t)segments) * m + m % 2 + 1;

        indices.push_back((unsigned int)dt);
        indices.push_back((unsigned int)left);
        indices.push_back((unsigned int)right);

        TB = calcTangentBitangent((unsigned int)dt, (unsigned int)left, (unsigned int)right);

        vertices[dt].Tangent += TB.first;
        vertices[dt].Bitangent += TB.second;

        vertices[left].Tangent += TB.first;
        vertices[left].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;
    }

    for (size_t i = start; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i - start];

        if (glm::length(vertices[i].Tangent) != 0.f) {
            vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
        }

        vertices[i].Bitangent /= (float)trisNum[i - start];

        if (glm::length(vertices[i].Bitangent) != 0.f) {
            vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
        }
    }

    trisNum.clear();

    generateCircle(segments, -h * 0.5f, CylinderCullFace::BACK, range);
}

Cylinder::Cylinder(unsigned int segments, CylinderShading shading, ValuesRange range)
{
    vertices.clear();
    indices.clear();
    generate(std::max(3u, segments), range, shading == CylinderShading::FLAT);
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