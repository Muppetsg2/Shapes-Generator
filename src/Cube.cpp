#include "Cube.hpp"
#include "BitMathOperators.hpp"

void Cube::_generate(const ValuesRange range)
{
    //https://catonif.github.io/cube/
    /*

        4       3
    1       2


        7       8
    5       6

    Pos (Every the same. Like normals but combined and from -0.5 to 0.5)
        [((int)((i + 1) / 2) % 2 - ((int)((i + 1) / 2) + 1) % 2) * 0.5, ((int)((i + 4) / 4) % 2 - (int)(i / 4)) * 0.5, ((((int)(i / 2) + 1) % 2) - ((int)(i / 2) % 2)) * 0.5]
        first   8 [(-111-1-111-1), (1111-1-1-1-1), (11-1-111-1-1)] * 0.5
        second  8 [(-111-1-111-1), (1111-1-1-1-1), (11-1-111-1-1)] * 0.5
        third   8 [(-111-1-111-1), (1111-1-1-1-1), (11-1-111-1-1)] * 0.5
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

    const Config& config = get_config();
    std::vector<unsigned int> trisNum;

    for (unsigned int p = 0u; p < 3u; ++p) {
        for (unsigned int i = 0u; i < 8u; ++i) {

            const float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;
            const glm::vec3 pos = {
                (float)((int)mod_2(div_2(i + 1u)) - (int)mod_2(div_2(i + 1u) + 1u)) * mult,
                (float)((int)mod_2(div_4(i + 4u)) - (int)div_4(i)) * mult,
                (float)((int)mod_2(div_2(i) + 1u) - (int)mod_2(div_2(i))) * mult
            };

            glm::vec2 tex = glm::vec2(0.f);
            glm::vec3 norm = glm::vec3(0.f);
            unsigned int tris = 1;

            switch (p) {
                case 0u : {
                    tex = {
                        (float)mod_2(i),
                        (float)div_4(i)
                    };

                    norm = {
                        0.f,
                        0.f,
                        (float)((int)mod_2(div_2(i) + 1u) - (int)mod_2(div_2(i)))
                    };

                    tris = mod_2(i + mod_2(div_4(i + 4u))) + 1u;
                    break;
                }
                case 1u : {
                    tex = {
                        (float)mod_2(i + 1u),
                        (float)div_4(i)
                    };

                    norm = {
                        (float)((int)mod_2(div_2(i + 1u)) - (int)mod_2(div_2(i + 1u) + 1u)),
                        0.f,
                        0.f
                    };

                    tris = mod_2(i + div_4(i)) + 1u;
                    break;
                }
                case 2u : {
                    tex = {
                        (float)mod_2(div_2(i + 1u)),
                        (float)mod_2(div_2(i + mul_2(mod_2(div_4(i + 4u)))))
                    };

                    norm = {
                        0.f,
                        (float)((int)mod_2(div_4(i + 4u)) - (int)div_4(i)),
                        0.f
                    };

                    tris = mod_2(i + div_4(i)) + 1u;
                    break;
                }
                default : {
                    break;
                }
            }

            _vertices.push_back({ pos, tex, norm, glm::vec3(0.f), glm::vec3(0.f) });
            if (config.genTangents) trisNum.push_back(tris);
        }
    }

    std::pair<glm::vec3, glm::vec3> TB;
    for (unsigned int p = 0u; p < 3u; ++p) {
        const unsigned int t1 = p + mul_8(p) + div_2(p);
        const unsigned int t2 = p + 2u + mul_8(p);

        unsigned int f, s, t; // First, Second, Third
        if (p == 0u) {
            for (unsigned int i = 0u; i < 2u; ++i) {
                const unsigned int v = i == 0u ? t1 : t2;

                // First Triangle
                f = v;
                s = v + 4u;
                t = v + 5u;

                _indices.push_back(f);
                _indices.push_back(s);
                _indices.push_back(t);

                if (config.genTangents) {
                    TB = _calcTangentBitangent(f, s, t);

                    _vertices[f].Tangent += TB.first;
                    _vertices[f].Bitangent += TB.second;

                    _vertices[s].Tangent += TB.first;
                    _vertices[s].Bitangent += TB.second;

                    _vertices[t].Tangent += TB.first;
                    _vertices[t].Bitangent += TB.second;
                }

                // Second Triangle
                s = v + 5u;
                t = v + 1u;

                _indices.push_back(f);
                _indices.push_back(s);
                _indices.push_back(t);

                if (config.genTangents) {
                    TB = _calcTangentBitangent(f, s, t);

                    _vertices[f].Tangent += TB.first;
                    _vertices[f].Bitangent += TB.second;

                    _vertices[s].Tangent += TB.first;
                    _vertices[s].Bitangent += TB.second;

                    _vertices[t].Tangent += TB.first;
                    _vertices[t].Bitangent += TB.second;
                }
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

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // Second Triangle
            s = t1 + 5u;
            t = t1 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // LEFT
            // First Triangle
            f = t2;
            s = t2 + 4u;
            t = t2 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // Second Triangle
            s = t2 + 1u;
            t = t2 - 3u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }
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

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // Second Triangle
            s = t1 - 2u;
            t = t1 - 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // BOTTOM
            // First Triangle
            f = t2;
            s = t2 + 3u;
            t = t2 + 2u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }

            // Second Triangle
            s = t2 + 2u;
            t = t2 + 1u;

            _indices.push_back(f);
            _indices.push_back(s);
            _indices.push_back(t);

            if (config.genTangents) {
                TB = _calcTangentBitangent(f, s, t);

                _vertices[f].Tangent += TB.first;
                _vertices[f].Bitangent += TB.second;

                _vertices[s].Tangent += TB.first;
                _vertices[s].Bitangent += TB.second;

                _vertices[t].Tangent += TB.first;
                _vertices[t].Bitangent += TB.second;
            }
        }
    }

    if (config.genTangents) _normalizeTangents(trisNum, 0ull, _vertices.size());

    trisNum.clear();
}

Cube::Cube(const ValuesRange range)
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