#include "pch.hpp"
#include "Torus.hpp"
#include "Shape.hpp"
#include "Constants.hpp"
#include "Vertex.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

glm::vec3 Torus::_getAverageNormal(const glm::vec3 n1, const glm::vec3 n2, const glm::vec3 n3) const
{
    const glm::vec3 average = n1 + n2 + n3;
    return fabsf(glm::length(average)) >= EPSILON ? glm::normalize(average) : average;
}

void Torus::_generate(const unsigned int segments, const unsigned int cs_segments, const float radius, const float cs_radius, const ValuesRange range, const bool useFlatShading)
{
    // Helpful 
    // https://gamedev.stackexchange.com/questions/16845/how-do-i-generate-a-torus-mesh

    const float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

    const float angleincs = 2.f * (float)M_PI / (float)segments;
    const float cs_angleincs = 2.f * (float)M_PI / (float)cs_segments;
    const float maxradius = radius + cs_radius;

    /* iterate cs_sides: inner ring */
    for (unsigned int j = 0u; j < cs_segments + 1u; ++j) {
        const float radJ = (float)j * cs_angleincs;
        const float currentradius = radius + (cs_radius * cosf(radJ));
        const float yval = cs_radius * sinf(radJ);

        /* iterate sides: outer ring */
        for (unsigned int i = 0u; i < segments + 1u; ++i) {
            const float radI = (float)i * angleincs;

            const float u = (float)i * angleincs * 0.5f * (float)M_1_PI;
            float v = ((float)j * cs_angleincs * (float)M_1_PI) - 1.f;
            if (v < 0.f) v = -v;

            const float xc = radius * cosf(radI);
            const float zc = radius * sinf(radI);

            const glm::vec3 pos = glm::vec3(currentradius * cosf(radI), yval, currentradius * sinf(radI));
            const glm::vec3 n = glm::vec3(_map(pos.x, -maxradius, maxradius, -1.f, 1.f), _map(pos.y, -maxradius, maxradius, -1.f, 1.f), _map(pos.z, -maxradius, maxradius, -1.f, 1.f));
            _vertices.push_back({ n * mult, { u, v }, glm::normalize(glm::vec3(pos.x - xc, pos.y, pos.z - zc)), glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    if (useFlatShading) {
        /* inner ring */
        glm::vec3 tangent;
        std::vector<Vertex> tempVertices(_vertices);
        _vertices.clear();
        for (unsigned int i = 0u; i < cs_segments; ++i) {
            const unsigned int nextrow = segments + 1u;

            /* outer ring */
            for (unsigned int j = 0u; j < segments; ++j) {
                const unsigned int first = i * nextrow + j;
                const unsigned int second = i * nextrow + j + 1u;
                const unsigned int third = i * nextrow + j + nextrow;
                const unsigned int fourth = i * nextrow + j + nextrow + 1u;

                const size_t start = _vertices.size();
                unsigned int f = (unsigned int)start;
                unsigned int s = (unsigned int)start + 1u;
                unsigned int t = (unsigned int)start + 2u;

                for (int tri = 0; tri < 2; ++tri) {
                    const bool isFirst = tri == 0;

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

                    if (_shapeConfig.genTangents) {
                        tangent = _calcTangent(f, s, t);

                        _vertices[f].Tangent = tangent;
                        _normalizeTangentAndGenerateBitangent(f);

                        _vertices[s].Tangent = tangent;
                        _normalizeTangentAndGenerateBitangent(s);

                        _vertices[t].Tangent = tangent;
                        _normalizeTangentAndGenerateBitangent(t);
                    }

                    f += 3u;
                    s += 3u;
                    t += 3u;
                }
            }
        }
    }
    else {
        /* inner ring */
        glm::vec3 tangent;
        for (unsigned int i = 0u; i < cs_segments; ++i) {
            const unsigned int nextrow = segments + 1u;

            /* outer ring */
            for (unsigned int j = 0u; j < segments; ++j) {
                const unsigned int first = i * nextrow + j;
                const unsigned int second = i * nextrow + j + 1u;
                const unsigned int third = i * nextrow + j + nextrow;
                const unsigned int fourth = i * nextrow + j + nextrow + 1u;

                // first triangle
                _indices.push_back(third);
                _indices.push_back(second);
                _indices.push_back(first);

                // second triangle
                _indices.push_back(second);
                _indices.push_back(third);
                _indices.push_back(fourth);

                if (_shapeConfig.genTangents) {
                    // first triangle
                    tangent = _calcTangent(third, second, first);

                    _vertices[third].Tangent += tangent;
                    _vertices[second].Tangent += tangent;
                    _vertices[first].Tangent += tangent;

                    // second triangle
                    tangent = _calcTangent(second, third, fourth);

                    _vertices[second].Tangent += tangent;
                    _vertices[third].Tangent += tangent;
                    _vertices[fourth].Tangent += tangent;
                }
            }
        }

        if (_shapeConfig.genTangents) {
            std::vector<unsigned int> trisNum(_vertices.size(), 0u);

            const size_t indexCount = _indices.size();
            for (size_t i = 0ull; i < indexCount; ++i) {
                ++trisNum[_indices[i]];
            }

            _normalizeTangentsAndGenerateBitangents(trisNum, 0ull, _vertices.size());
            trisNum.clear();
        }
    }
}

Torus::Torus(const ShapeConfig& config, const unsigned int segments, const unsigned int cs_segments, const float radius, const float cs_radius, const TorusShading shading, const ValuesRange range)
{
    _shapeConfig = config;
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
