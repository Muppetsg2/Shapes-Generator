#include "Plane.h"

float Plane::fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd) {
    return expectedStart + ((expectedEnd - expectedStart) / (currEnd - currStart)) * (input - currStart);
}

void Plane::generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir)
{
    float diffX = 1.f / (columns - 1);
    float diffZ = 1.f / (rows - 1);

    std::vector<unsigned int> triangleNumForVert;

    for (int row = 0; row < rows; ++row) {

        float z = -.5f + row * diffZ;

        for (int col = 0; col < columns; ++col) {
            float x = -.5f + col * diffX;

            switch (dir) {
                case UP:
                {
                    vertices.push_back({ { x, 0.f, z }, { fmapf(x, -.5f, .5f, 0.f, 1.f), fmapf(z, -.5f, .5f, 0.f, 1.f) }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
                case FRONT:
                {
                    vertices.push_back({ { x, z, 0.f }, { fmapf(x, -.5f, .5f, 0.f, 1.f), fmapf(z, -.5f, .5f, 0.f, 1.f) }, { 0.f, 0.f, 1.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
                default:
                {
                    vertices.push_back({ { x, 0.f, z }, { fmapf(x, -.5f, .5f, 0.f, 1.f), fmapf(z, -.5f, .5f, 0.f, 1.f) }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
                    break;
                }
            }

            if (row * columns + col == 0 || row * columns + col == rows * columns - 1) {
                triangleNumForVert.push_back(1);
            }
            else if ((row == 0 && col + 1 == columns) || (row + 1 == rows && col == 0)) {
                triangleNumForVert.push_back(2);
            }
            else if (row == 0 || row + 1 == rows || col == 0 || col + 1 == columns) {
                triangleNumForVert.push_back(3);
            }
            else {
                triangleNumForVert.push_back(6);
            }
        }
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {

        if ((i + 1) % columns == 0) {
            continue;
        }

        if (i + columns >= vertices.size()) {
            break;
        }

        // First Triangle
        indices.push_back(i + columns);
        indices.push_back(i + 1);
        indices.push_back(i);

        std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(i + columns, i + 1, i);

        vertices[i + columns].Tangent += TB.first;
        vertices[i + columns].Bitangent += TB.second;

        vertices[i + 1].Tangent += TB.first;
        vertices[i + 1].Bitangent += TB.second;

        vertices[i].Tangent += TB.first;
        vertices[i].Bitangent += TB.second;

        // Second Triangle
        indices.push_back(i + columns);
        indices.push_back(i + columns + 1);
        indices.push_back(i + 1);

        TB = calcTangentBitangent(i + columns, i + columns + 1, i + 1);

        vertices[i + columns].Tangent += TB.first;
        vertices[i + columns].Bitangent += TB.second;

        vertices[i + columns + 1].Tangent += TB.first;
        vertices[i + columns + 1].Bitangent += TB.second;

        vertices[i + 1].Tangent += TB.first;
        vertices[i + 1].Bitangent += TB.second;
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)triangleNumForVert[i];
        vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

        vertices[i].Bitangent /= (float)triangleNumForVert[i];
        vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
    }
}

Plane::Plane(unsigned int rows, unsigned int columns, PlaneNormalDir dir)
{
    generate(rows >= 2 ? rows : 2, columns >= 2 ? columns : 2, dir);
}

Plane::~Plane() {}