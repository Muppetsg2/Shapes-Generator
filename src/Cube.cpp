#include "Cube.hpp"

void Cube::_generate(ValuesRange range)
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

            float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;
            glm::vec3 pos = {
                (float)((int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2) * mult,
                (float)((int)((i + 4) / 4) % 2 - (int)(i / 4)) * mult,
                (float)((((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)) * mult
            };

            glm::vec2 tex = glm::vec2(0.f);
            glm::vec3 norm = glm::vec3(0.f);
            unsigned int tris = 1;

            switch (p) {
                case 0 : {
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
                case 1 : {
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
                case 2 : {
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
                default : {
                    break;
                }
            }

            _vertices.push_back({ pos, tex, norm, glm::vec3(0.f), glm::vec3(0.f) });
            trisNum.push_back(tris);
        }
    }

    std::pair<glm::vec3, glm::vec3> TB;
    for (unsigned int p = 0u; p < 3u; ++p) {
        unsigned int t1 = p + 8u * p + (unsigned int)(p / 2u);
        unsigned int t2 = p + 2u + 8u * p;

        unsigned int f, s, t; // First, Second, Third
        if (p == 0u) {
            for (unsigned int i = 0u; i < 2u; ++i) {
                unsigned int v = i == 0u ? t1 : t2;

                // First Triangle
                f = v;
                s = v + 4u;
                t = v + 5u;

                _indices.push_back(f);
                _indices.push_back(s);
                _indices.push_back(t);

                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;

                // Second Triangle
                s = v + 5u;
                t = v + 1u;

                _indices.push_back(f);
                _indices.push_back(s);
                _indices.push_back(t);

                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }
        }
        else if (p == 1u) {
            // RIGHT
            // First Triangle
            f = t1;
            s = t1 + 4u;
            t = t1 + 5u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // Second Triangle
            s = t1 + 5u;
            t = t1 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // LEFT
            // First Triangle
            f = t2;
            s = t2 + 4u;
            t = t2 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // Second Triangle
            s = t2 + 1u;
            t = t2 - 3u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;
        }
        else if (p == 2u) {
            // TOP
            // First Triangle
            f = t1;
            s = t1 - 3u;
            t = t1 - 2u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // Second Triangle
            s = t1 - 2u;
            t = t1 - 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // BOTTOM
            // First Triangle
            f = t2;
            s = t2 + 3u;
            t = t2 + 2u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;

            // Second Triangle
            s = t2 + 2u;
            t = t2 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            TB = _calcTangentBitangent(f, s, t);

            _vertices[f].Tangent += TB.first;
            _vertices[f].Bitangent += TB.second;

            _vertices[s].Tangent += TB.first;
            _vertices[s].Bitangent += TB.second;

            _vertices[t].Tangent += TB.first;
            _vertices[t].Bitangent += TB.second;
        }
    }

    _normalizeTangents(trisNum, 0ull, _vertices.size());

    trisNum.clear();
}

Cube::Cube(ValuesRange range)
{
    _vertices.clear();
    _indices.clear();
    _generate(range);
}

Cube::~Cube() {}

std::string Cube::getClassName()
{
    return "Cube";
}

std::string Cube::getObjectClassName() const
{
    return Cube::getClassName();
}