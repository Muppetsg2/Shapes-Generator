#include "Hexagon.h"
#include <format>

pair<glm::vec3, glm::vec3> Hexagon::calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3)
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

string Hexagon::getHexagonAsString()
{
    string text = "std::vector<Vertex> vertices {\n";

    for (int i = 0; i < vertices.size(); ++i) {
        Vertex v = vertices[i];

        text += vformat(string_view("\t{} {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{} {}"),
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