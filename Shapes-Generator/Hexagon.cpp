#include "Hexagon.h"

void Hexagon::generateCircle(unsigned int segments, float y, unsigned int cullFace)
{
    std::vector<unsigned int> trisNum;

    float angleXZDiff = 360.f / (float)segments;

    // CIRCLE TOP
    // VERTICES AND TEX COORDS
    unsigned int start = vertices.size();

    float angleXZ = 0.f;
    for (unsigned int j = 0; j < segments; ++j) {
        float radiansXZ = glm::radians(angleXZ);
        float z = cosf(radiansXZ);
        float x = sinf(radiansXZ);
        vertices.push_back({ { x, y, z }, { .5f + x * .5f, .5f + z * .5f }, (cullFace == 0 ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
        trisNum.push_back(2);
        angleXZ += angleXZDiff;
    }
    vertices.push_back({ { 0.f, y, 0.f }, { .5f, .5f }, (cullFace == 0 ? glm::vec3(0.f, 1.f, 0.f) : glm::vec3(0.f, -1.f, 0.f)), glm::vec3(0.f), glm::vec3(0.f) });
    trisNum.push_back(segments);

    // INDICES
    for (unsigned int i = start; i < vertices.size() - 1; ++i) {

        unsigned int right = i + 2 == vertices.size() ? start : i + 1;

        indices.push_back(cullFace == 0 ? i : right);
        indices.push_back(cullFace == 0 ? right : i);
        indices.push_back(vertices.size() - 1);

        std::pair<glm::vec3, glm::vec3> TB;

        if (cullFace == 0) {
            TB = calcTangentBitangent(i, right, vertices.size() - 1);
        }
        else {
            TB = calcTangentBitangent(right, i, vertices.size() - 1);
        }

        vertices[i].Tangent += TB.first;
        vertices[i].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;

        vertices[vertices.size() - 1].Tangent += TB.first;
        vertices[vertices.size() - 1].Bitangent += TB.second;
    }

    for (unsigned int i = start; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i - start];
        vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

        vertices[i].Bitangent /= (float)trisNum[i - start];
        vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
    }

    trisNum.clear();
}

Hexagon::Hexagon()
{
    vertices.clear();
    indices.clear();

    int segments = 6;
    float h = 1.f;

    generateCircle(segments, h / 2.f, 0);

    std::vector<unsigned int> trisNum;

    float angleXZDiff = 360.f / (float)segments;

    unsigned int start = vertices.size();

    // VERTICES UP AND DOWN
    for (int i = 0; i < 2; ++i) {
        float y = h / 2.f - h * i;
        float angleXZ = 0.f;
        for (unsigned int j = 0; j < segments; ++j) {

            float x_n = (sinf(glm::radians(angleXZ)) + sinf(glm::radians(angleXZ + angleXZDiff))) / 2.f;
            float z_n = (cosf(glm::radians(angleXZ)) + cosf(glm::radians(angleXZ + angleXZDiff))) / 2.f;

            for (int f = 0; f < 2; ++f) {
                float angle = angleXZ + angleXZDiff * f;
                float radiansXZ = glm::radians(angle);
                float z = cosf(radiansXZ);
                float x = sinf(radiansXZ);
                vertices.push_back({ { x, y, z }, { (float)f, (float)i}, glm::normalize(glm::vec3(x_n, 0.f, z_n)), glm::vec3(0.f), glm::vec3(0.f) });

                trisNum.push_back(1 + (i + f) % 2);
            }
            angleXZ += angleXZDiff;
        }
    }

    // INDICES
    for (unsigned int i = 0; i < segments; ++i) {

        int left = start + i * 2;
        int dt = start + i * 2 + segments * 2;
        int right = start + i * 2 + 1;

        indices.push_back(left);
        indices.push_back(dt);
        indices.push_back(right);

        std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(left, dt, right);

        vertices[left].Tangent += TB.first;
        vertices[left].Bitangent += TB.second;

        vertices[dt].Tangent += TB.first;
        vertices[dt].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;

        dt = start + i * 2 + 1;
        left = start + i * 2 + segments * 2;
        right = start + i * 2 + segments * 2 + 1;

        indices.push_back(dt);
        indices.push_back(left);
        indices.push_back(right);

        TB = calcTangentBitangent(dt, left, right);

        vertices[dt].Tangent += TB.first;
        vertices[dt].Bitangent += TB.second;

        vertices[left].Tangent += TB.first;
        vertices[left].Bitangent += TB.second;

        vertices[right].Tangent += TB.first;
        vertices[right].Bitangent += TB.second;
    }

    for (unsigned int i = start; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i - start];
        vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

        vertices[i].Bitangent /= (float)trisNum[i - start];
        vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
    }

    trisNum.clear();

    generateCircle(segments, -h / 2.f, 1);
}

Hexagon::~Hexagon() {}