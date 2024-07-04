#include "Plane.h"
#include <format>

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

float Plane::fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd) {
    return expectedStart + ((expectedEnd - expectedStart) / (currEnd - currStart)) * (input - currStart);
}

Vertex Plane::calcTangentBitangent(unsigned int vertexIndex)
{
    Vertex v = vertices[vertexIndex];

    glm::vec3 tangent = glm::vec3(0.f);
    glm::vec3 bitangent = glm::vec3(0.f);
    unsigned int trianglesIncluded = 0;

    // Find the triangles that use v
    //  * Loop over every triangle (i + 3)
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        // Only perform the calculation if one of the indices
        // matches our vertexIndex
        if (index0 == vertexIndex || index1 == vertexIndex || index2 == vertexIndex) {
            Vertex v0 = vertices[index0];
            Vertex v1 = vertices[index1];
            Vertex v2 = vertices[index2];

            glm::vec3 pos0 = v0.Position;
            glm::vec3 pos1 = v1.Position;
            glm::vec3 pos2 = v2.Position;

            glm::vec2 uv0 = v0.TexCoord;
            glm::vec2 uv1 = v1.TexCoord;
            glm::vec2 uv2 = v2.TexCoord;

            glm::vec3 delta_pos1 = pos1 - pos0;
            glm::vec3 delta_pos2 = pos2 - pos0;

            glm::vec2 delta_uv1 = uv1 - uv0;
            glm::vec2 delta_uv2 = uv2 - uv0;

            float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

            tangent += (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;

            bitangent += (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

            trianglesIncluded += 1;
        }

    }

    // Average the tangent and bitangents
    if (trianglesIncluded > 0) {
        tangent /= trianglesIncluded;
        bitangent /= trianglesIncluded;
        tangent = normalize(tangent);
        bitangent = normalize(bitangent);
    }

    // Save the results
    v.Tangent = tangent;
    v.Bitangent = bitangent;

    return v;
}

std::pair<glm::vec3, glm::vec3> Plane::calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3)
{
    std::pair<glm::vec3, glm::vec3> TB;

    Vertex v0 = vertices[t1];
    Vertex v1 = vertices[t2];
    Vertex v2 = vertices[t3];

    glm::vec3 pos0 = v0.Position;
    glm::vec3 pos1 = v1.Position;
    glm::vec3 pos2 = v2.Position;

    glm::vec2 uv0 = v0.TexCoord;
    glm::vec2 uv1 = v1.TexCoord;
    glm::vec2 uv2 = v2.TexCoord;

    glm::vec3 delta_pos1 = pos1 - pos0;
    glm::vec3 delta_pos2 = pos2 - pos0;

    glm::vec2 delta_uv1 = uv1 - uv0;
    glm::vec2 delta_uv2 = uv2 - uv0;

    float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

    // Save the results
    TB.first = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
    TB.second = (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

    return TB;
}

Plane::Plane(unsigned int rows, unsigned int columns, PlaneNormalDir dir)
{
    if (rows < 2 || columns < 2) {
        generate(2, 2, dir);
        return;
    }

    generate(rows, columns, dir);
}

string Plane::getPlaneAsString()
{
    string text = "std::vector<Vertex> vertices {\n";

    for (int i = 0; i < vertices.size(); ++i) {
        Vertex v = vertices[i];

        text += vformat(string_view("\t{} {}{:.1f}f, {:.1f}f, {:.1f}f{}, {}{:.1f}f, {:.1f}f{}, {}{:.1f}f, {:.1f}f, {:.1f}f{}, {}{:.1f}f, {:.1f}f, {:.1f}f{}, {}{:.1f}f, {:.1f}f, {:.1f}f{} {}"),
            make_format_args
            (
                "{", ".Position = glm::vec3(",
                v.Position.x, v.Position.y, v.Position.z,
                ")", ".TexCoords = glm::vec2(",
                v.TexCoord.x, v.TexCoord.y,
                ")", ".Normal = glm::vec3(",
                v.Normal.x, v.Normal.y, v.Normal.z,
                ")", ".Tangent = glm::vec3(",
                v.Tangent.x, v.Tangent.y, v.Tangent.z,
                ")", ".Bitangent = glm::vec3(",
                v.Bitangent.x, v.Bitangent.y, v.Bitangent.z,
                ")", "}"
            )
        );

        if (i + 1 < vertices.size()) {
            text += ",\n";
        }
        else {
            text += "\n";
        }
    }

    text += "};\n\nstd::vector<unsigned int> indices = {\n";

    for (int i = 0; i < indices.size(); i += 3) {
        text += vformat
        (
            string_view("\t{0}, {1}, {2}"),
            make_format_args
            (
                indices[i],
                indices[i + 1],
                indices[i + 2]
            )
        );

        if (i + 3 < indices.size()) {
            text += ",\n";
        }
        else {
            text += "\n";
        }
    }

    text += "};";

    return text;
}
