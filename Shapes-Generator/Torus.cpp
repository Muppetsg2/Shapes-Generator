// PRECOMPILED HEADER
#include "pch.h"
#include "Torus.h"
#include <map>

void Torus::generate(unsigned int segments, unsigned int cs_segments, float radius, float cs_radius, ValuesRange range)
{
    // Helpful 
    // https://gamedev.stackexchange.com/questions/16845/how-do-i-generate-a-torus-mesh

    float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

    float angleincs = 360.f / (float)segments;
    float cs_angleincs = 360.f / (float)cs_segments;
    float maxradius = radius + cs_radius;

    /* iterate cs_sides: inner ring */
    for (unsigned int j = 0; j < cs_segments + 1; ++j)
    {
        float radJ = glm::radians((float)j * cs_angleincs);
        float currentradius = radius + (cs_radius * std::cosf(radJ));
        float yval = cs_radius * std::sinf(radJ);

        /* iterate sides: outer ring */
        for (unsigned int i = 0; i < segments + 1; ++i)
        {
            float radI = glm::radians((float)i * angleincs);

            float u = ((float)i * angleincs) / 360.f;
            float v = ((2.f * (float)j * cs_angleincs) / 360.f) - 1.f;
            if (v < 0.f) v = -v;

            float xc = radius * std::cosf(radI);
            float zc = radius * std::sinf(radI);

            glm::vec3 pos = glm::vec3(currentradius * std::cosf(radI), yval, currentradius * std::sinf(radI));
            glm::vec3 n = glm::vec3(fmapf(pos.x, -maxradius, maxradius, -1.f, 1.f), fmapf(pos.y, -maxradius, maxradius, -1.f, 1.f), fmapf(pos.z, -maxradius, maxradius, -1.f, 1.f));
            vertices.push_back({ n * mult, { u, v }, glm::normalize(glm::vec3(pos.x - xc, pos.y, pos.z - zc)), glm::vec3(0.f), glm::vec3(0.f) });
        }
    }

    /* inner ring */
    for (unsigned int i = 0; i < cs_segments; ++i)
    {
        unsigned int nextrow = segments + 1;

        // outer ring
        for (unsigned int j = 0; j < segments; ++j)
        {
            unsigned int first = i * nextrow + j;
            unsigned int second = i * nextrow + j + 1;
            unsigned int third = i * nextrow + j + nextrow;
            unsigned int fourth = i * nextrow + j + nextrow + 1;

            indices.push_back(third);
            indices.push_back(second);
            indices.push_back(first);

            std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(third, second, first);

            vertices[third].Tangent += TB.first;
            vertices[third].Bitangent += TB.second;

            vertices[second].Tangent += TB.first;
            vertices[second].Bitangent += TB.second;

            vertices[first].Tangent += TB.first;
            vertices[first].Bitangent += TB.second;

            indices.push_back(second);
            indices.push_back(third);
            indices.push_back(fourth);

            TB = calcTangentBitangent(second, third, fourth);

            vertices[second].Tangent += TB.first;
            vertices[second].Bitangent += TB.second;

            vertices[third].Tangent += TB.first;
            vertices[third].Bitangent += TB.second;

            vertices[fourth].Tangent += TB.first;
            vertices[fourth].Bitangent += TB.second;
        }
    }

    std::map<unsigned int, unsigned int> trisNum;

    for (unsigned int i : indices) {
        trisNum[i] += 1;
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        vertices[i].Tangent /= (float)trisNum[i];

        if (glm::length(vertices[i].Tangent) != 0.f) {
            vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
        }

        vertices[i].Bitangent /= (float)trisNum[i];

        if (glm::length(vertices[i].Bitangent) != 0.f) {
            vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
        }
    }

    trisNum.clear();
}

Torus::Torus(unsigned int segments, unsigned int cs_segments, float radius, float cs_radius, ValuesRange range)
{
	vertices.clear();
	indices.clear();
	generate(std::max(3u, segments), std::max(3u, cs_segments), std::max(1e-6f, radius), std::max(1e-6f, cs_radius), range);
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
