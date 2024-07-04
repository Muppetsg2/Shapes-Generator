#include "Cube.h"
#include <format>

std::pair<glm::vec3, glm::vec3> Cube::calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3)
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

Cube::Cube()
{
    //https://catonif.github.io/cube/

    /*

        4       3
    1       2


        7       8
    5       6

    Pos (Every the same. Like normals but combined and from -0.5 to 0.5)
        [((int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2) * 0.5, ((int)((i + 4) / 4) % 2 - (int)(i / 4)) * 0.5, ((((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)) * 0.5]
        first 8
        second 8
        third 8
    */

    /*
    TexCoord
        for first  8 [i % 2, (int)(i / 4)] for pattern [(01010101), (00001111)]
        for second 8 [(i + 1) % 2, (int)(i / 4)] for pattern [(10101010), (00001111)]
        for third  8 [(int)((i + 1) / 2) % 2, (int)((i + 2 * ((int)((i + 4) / 4) % 2)) / 2) % 2] for pattern [(01100110), (11000011)]
    */

    /*
    Norms
        for first  8 [0, 0, (((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)] for pattern [0, 0, (11-1-111-1-1)]
        for second 8 [(int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2, 0, 0] for pattern [(-111-1-111-1), 0, 0]
        for third  8 [0, (int)((i + 4) / 4) % 2 - (int)(i / 4), 0] for pattern [0, (1111-1-1-1-1), 0]
    */

    /*
    TrisNum
        for first        8 ((i + 1 * ((int)((i + 4) / 4) % 2)) % 2) + 1 for pattern (21211212)
        for second/third 8 ((i + 1 * (int)(i / 4)) % 2) + 1 for pattern (12122121)
    */

    /*
    Indicies
    [p + 8 * p + (int)(p / 2)]
    0
    9
    19

    [p + 2 + 8 * p]
    2
    11
    20

    Triangles from starting points
    p == 0
    0, 0 + 4, 0 + 5
    0, 0 + 5, 0 + 1

    2, 2 + 4, 2 + 5
    2, 2 + 5, 2 + 1


    p == 1
    9, 9 + 4, 9 + 5
    9, 9 + 5, 9 + 1

    11, 11 + 4, 11 + 1
    11, 11 + 1, 11 - 3


    p == 2
    19, 19 - 3, 19 - 2
    19, 19 - 2, 19 - 1

    20, 20 + 3, 20 + 2
    20, 20 + 2, 20 + 1
    */

    std::vector<unsigned int> trisNum;

    for (int p = 0; p < 3; ++p) {
        for (int i = 0; i < 8; ++i) {

            glm::vec3 pos = {
                ((int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2) * .5f,
                ((int)((i + 4) / 4) % 2 - (int)(i / 4)) * .5f,
                ((((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)) * .5f
            };

            glm::vec2 tex = glm::vec2(0.f);
            glm::vec3 norm = glm::vec3(0.f);
            unsigned int tris = 1;

            switch (p) {
            case 0: {
                tex = {
                    (float)(i % 2),
                    (float)((int)(i / 4))
                };

                norm = {
                    0.f,
                    0.f,
                    (float)(((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)
                };

                tris = ((i + 1 * ((int)((i + 4) / 4) % 2)) % 2) + 1;
                break;
            }
            case 1: {
                tex = {
                    (float)((i + 1) % 2),
                    (float)((int)(i / 4))
                };

                norm = {
                    (float)((int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2),
                    0.f,
                    0.f
                };

                tris = ((i + 1 * (int)(i / 4)) % 2) + 1;
                break;
            }
            case 2: {
                tex = {
                    (float)((int)((i + 1) / 2) % 2),
                    (float)((int)((i + 2 * ((int)((i + 4) / 4) % 2)) / 2) % 2)
                };

                norm = {
                    0.f,
                    (float)((int)((i + 4) / 4) % 2 - (int)(i / 4)),
                    0.f
                };

                tris = ((i + 1 * (int)(i / 4)) % 2) + 1;
                break;
            }
            default: {
                break;
            }
            }

            vertices.push_back({ pos, tex, norm, glm::vec3(0.f), glm::vec3(0.f) });
            trisNum.push_back(tris);
        }
    }

    for (int p = 0; p < 3; ++p) {
        int t1 = p + 8 * p + (int)(p / 2);
        int t2 = p + 2 + 8 * p;

        std::pair<glm::vec3, glm::vec3> TB;

        if (p == 0) {

            for (int i = 0; i < 2; ++i) {

                int v = i == 0 ? t1 : t2;

                // First Triangle
                indices.push_back(v);
                indices.push_back(v + 4);
                indices.push_back(v + 5);

                TB = calcTangentBitangent(v, v + 4, v + 5);

                vertices[v].Tangent += TB.first;
                vertices[v].Bitangent += TB.second;

                vertices[v + 4].Tangent += TB.first;
                vertices[v + 4].Bitangent += TB.second;

                vertices[v + 5].Tangent += TB.first;
                vertices[v + 5].Bitangent += TB.second;

                // Second Triangle
                indices.push_back(v);
                indices.push_back(v + 5);
                indices.push_back(v + 1);

                TB = calcTangentBitangent(v, v + 5, v + 1);

                vertices[v].Tangent += TB.first;
                vertices[v].Bitangent += TB.second;

                vertices[v + 5].Tangent += TB.first;
                vertices[v + 5].Bitangent += TB.second;

                vertices[v + 1].Tangent += TB.first;
                vertices[v + 1].Bitangent += TB.second;
            }
        }
        else if (p == 1) {
            // RIGHT
            // First Triangle
            indices.push_back(t1);
            indices.push_back(t1 + 4);
            indices.push_back(t1 + 5);

            TB = calcTangentBitangent(t1, t1 + 4, t1 + 5);

            vertices[t1].Tangent += TB.first;
            vertices[t1].Bitangent += TB.second;

            vertices[t1 + 4].Tangent += TB.first;
            vertices[t1 + 4].Bitangent += TB.second;

            vertices[t1 + 5].Tangent += TB.first;
            vertices[t1 + 5].Bitangent += TB.second;

            // Second Triangle
            indices.push_back(t1);
            indices.push_back(t1 + 5);
            indices.push_back(t1 + 1);

            TB = calcTangentBitangent(t1, t1 + 5, t1 + 1);

            vertices[t1].Tangent += TB.first;
            vertices[t1].Bitangent += TB.second;

            vertices[t1 + 5].Tangent += TB.first;
            vertices[t1 + 5].Bitangent += TB.second;

            vertices[t1 + 1].Tangent += TB.first;
            vertices[t1 + 1].Bitangent += TB.second;

            // LEFT
            // First Triangle
            indices.push_back(t2);
            indices.push_back(t2 + 4);
            indices.push_back(t2 + 1);

            TB = calcTangentBitangent(t2, t2 + 4, t2 + 1);

            vertices[t2].Tangent += TB.first;
            vertices[t2].Bitangent += TB.second;

            vertices[t2 + 4].Tangent += TB.first;
            vertices[t2 + 4].Bitangent += TB.second;

            vertices[t2 + 1].Tangent += TB.first;
            vertices[t2 + 1].Bitangent += TB.second;

            // Second Triangle
            indices.push_back(t2);
            indices.push_back(t2 + 1);
            indices.push_back(t2 - 3);

            TB = calcTangentBitangent(t2, t2 + 1, t2 - 3);

            vertices[t2].Tangent += TB.first;
            vertices[t2].Bitangent += TB.second;

            vertices[t2 + 1].Tangent += TB.first;
            vertices[t2 + 1].Bitangent += TB.second;

            vertices[t2 - 3].Tangent += TB.first;
            vertices[t2 - 3].Bitangent += TB.second;
        }
        else if (p == 2) {
            // TOP
            // First Triangle
            indices.push_back(t1);
            indices.push_back(t1 - 3);
            indices.push_back(t1 - 2);

            TB = calcTangentBitangent(t1, t1 - 3, t1 - 2);

            vertices[t1].Tangent += TB.first;
            vertices[t1].Bitangent += TB.second;

            vertices[t1 - 3].Tangent += TB.first;
            vertices[t1 - 3].Bitangent += TB.second;

            vertices[t1 - 2].Tangent += TB.first;
            vertices[t1 - 2].Bitangent += TB.second;

            // Second Triangle
            indices.push_back(t1);
            indices.push_back(t1 - 2);
            indices.push_back(t1 - 1);

            TB = calcTangentBitangent(t1, t1 - 2, t1 - 1);

            vertices[t1].Tangent += TB.first;
            vertices[t1].Bitangent += TB.second;

            vertices[t1 - 2].Tangent += TB.first;
            vertices[t1 - 2].Bitangent += TB.second;

            vertices[t1 - 1].Tangent += TB.first;
            vertices[t1 - 1].Bitangent += TB.second;

            // BOTTOM
            // First Triangle
            indices.push_back(t2);
            indices.push_back(t2 + 3);
            indices.push_back(t2 + 2);

            TB = calcTangentBitangent(t2, t2 + 3, t2 + 2);

            vertices[t2].Tangent += TB.first;
            vertices[t2].Bitangent += TB.second;

            vertices[t2 + 3].Tangent += TB.first;
            vertices[t2 + 3].Bitangent += TB.second;

            vertices[t2 + 2].Tangent += TB.first;
            vertices[t2 + 2].Bitangent += TB.second;

            // Second Triangle
            indices.push_back(t2);
            indices.push_back(t2 + 2);
            indices.push_back(t2 + 1);

            TB = calcTangentBitangent(t2, t2 + 2, t2 + 1);

            vertices[t2].Tangent += TB.first;
            vertices[t2].Bitangent += TB.second;

            vertices[t2 + 2].Tangent += TB.first;
            vertices[t2 + 2].Bitangent += TB.second;

            vertices[t2 + 1].Tangent += TB.first;
            vertices[t2 + 1].Bitangent += TB.second;
        }
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i];
        vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

        vertices[i].Bitangent /= (float)trisNum[i];
        vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
    }
}

string Cube::getCubeAsString()
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