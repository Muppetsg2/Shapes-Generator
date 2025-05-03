#include "pch.hpp"
#include "Torus.hpp"

glm::vec3 Torus::_getAverageNormal(glm::vec3 n1, glm::vec3 n2, glm::vec3 n3)
{
    glm::vec3 average = n1 + n2 + n3;
    return fabsf(glm::length(average)) >= EPSILON ? glm::normalize(average) : average;
}

void Torus::_generate(unsigned int segments, unsigned int cs_segments, float radius, float cs_radius, ValuesRange range, bool useFlatShading)
{
    // Helpful 
    // https://gamedev.stackexchange.com/questions/16845/how-do-i-generate-a-torus-mesh

    float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

    float angleincs = 2.f * (float)M_PI / (float)segments;
    float cs_angleincs = 2.f * (float)M_PI / (float)cs_segments;
    float maxradius = radius + cs_radius;

    /* iterate cs_sides: inner ring */
    for (unsigned int j = 0u; j < cs_segments + 1u; ++j) {
        float radJ = (float)j * cs_angleincs;
        float currentradius = radius + (cs_radius * cosf(radJ));
        float yval = cs_radius * sinf(radJ);

        /* iterate sides: outer ring */
        for (unsigned int i = 0u; i < segments + 1u; ++i) {
            float radI = (float)i * angleincs;

            float u = (float)i * angleincs * 0.5f * (float)M_1_PI;
            float v = ((float)j * cs_angleincs * (float)M_1_PI) - 1.f;
            if (v < 0.f) v = -v;

            float xc = radius * cosf(radI);
            float zc = radius * sinf(radI);

            glm::vec3 pos = glm::vec3(currentradius * cosf(radI), yval, currentradius * sinf(radI));
            glm::vec3 n = glm::vec3(_map(pos.x, -maxradius, maxradius, -1.f, 1.f), _map(pos.y, -maxradius, maxradius, -1.f, 1.f), _map(pos.z, -maxradius, maxradius, -1.f, 1.f));
            _vertices.push_back({ n * mult, { u, v }, glm::normalize(glm::vec3(pos.x - xc, pos.y, pos.z - zc)), glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    if (useFlatShading) {
        /* inner ring */
        std::pair<glm::vec3, glm::vec3> TB;
        std::vector<Vertex> tempVertices(_vertices);
        _vertices.clear();
        for (unsigned int i = 0u; i < cs_segments; ++i) {
            unsigned int nextrow = segments + 1u;

            /* outer ring */
            for (unsigned int j = 0u; j < segments; ++j) {
                unsigned int first = i * nextrow + j;
                unsigned int second = i * nextrow + j + 1u;
                unsigned int third = i * nextrow + j + nextrow;
                unsigned int fourth = i * nextrow + j + nextrow + 1u;

                size_t start = _vertices.size();
                unsigned int f = (unsigned int)start;
                unsigned int s = (unsigned int)start + 1u;
                unsigned int t = (unsigned int)start + 2u;

                for (int tri = 0; tri < 2; ++tri) {
                    bool isFirst = tri == 0;

                    Vertex v1 = isFirst ? tempVertices[third] : tempVertices[second];
                    Vertex v2 = isFirst ? tempVertices[second] : tempVertices[third];
                    Vertex v3 = isFirst ? tempVertices[first] : tempVertices[fourth];

                    glm::vec3 norm = _getAverageNormal(v1.Normal, v2.Normal, v3.Normal);
                    
                    v1.Normal = norm;
                    v2.Normal = norm;
                    v3.Normal = norm;

                    _vertices.push_back(v1);
                    _vertices.push_back(v2);
                    _vertices.push_back(v3);

                    _indices.push_back(f);
                    _indices.push_back(s);
                    _indices.push_back(t);

                    TB = _calcTangentBitangent(f, s, t);

                    _vertices[f].Tangent = TB.first;
                    _vertices[f].Bitangent = TB.second;

                    _vertices[s].Tangent = TB.first;
                    _vertices[s].Bitangent = TB.second;

                    _vertices[t].Tangent = TB.first;
                    _vertices[t].Bitangent = TB.second;

                    f += 3u;
                    s += 3u;
                    t += 3u;
                }
            }
        }
    }
    else {
        /* inner ring */
        std::pair<glm::vec3, glm::vec3> TB;
        for (unsigned int i = 0u; i < cs_segments; ++i) {
            unsigned int nextrow = segments + 1u;

            /* outer ring */
            for (unsigned int j = 0u; j < segments; ++j) {
                unsigned int first = i * nextrow + j;
                unsigned int second = i * nextrow + j + 1u;
                unsigned int third = i * nextrow + j + nextrow;
                unsigned int fourth = i * nextrow + j + nextrow + 1u;

                _indices.push_back(third);
                _indices.push_back(second);
                _indices.push_back(first);

                TB = _calcTangentBitangent(third, second, first);

                _vertices[third].Tangent += TB.first;
                _vertices[third].Bitangent += TB.second;

                _vertices[second].Tangent += TB.first;
                _vertices[second].Bitangent += TB.second;

                _vertices[first].Tangent += TB.first;
                _vertices[first].Bitangent += TB.second;

                _indices.push_back(second);
                _indices.push_back(third);
                _indices.push_back(fourth);

                TB = _calcTangentBitangent(second, third, fourth);

                _vertices[second].Tangent += TB.first;
                _vertices[second].Bitangent += TB.second;

                _vertices[third].Tangent += TB.first;
                _vertices[third].Bitangent += TB.second;

                _vertices[fourth].Tangent += TB.first;
                _vertices[fourth].Bitangent += TB.second;
            }
        }

        std::vector<unsigned int> trisNum(_vertices.size(), 0u);

        const size_t indexCount = _indices.size();
        for (size_t i = 0ull; i < indexCount; ++i) {
            ++trisNum[_indices[i]];
        }

        _normalizeTangents(trisNum, 0ull, _vertices.size());

        trisNum.clear();
    }
}

Torus::Torus(unsigned int segments, unsigned int cs_segments, float radius, float cs_radius, TorusShading shading, ValuesRange range)
{
	_vertices.clear();
	_indices.clear();
    _generate(std::max(3u, segments), std::max(3u, cs_segments), std::max(EPSILON, radius), std::max(EPSILON, cs_radius), range, shading == TorusShading::FLAT);
}

Torus::~Torus() {}

std::string Torus::getClassName()
{
	return "Torus";
}

std::string Torus::getObjectClassName() const
{
	return Torus::getClassName();
}
