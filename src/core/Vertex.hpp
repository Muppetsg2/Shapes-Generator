#pragma once
#include "Constants.hpp"

#include <cmath>
#include <functional>

#include <glm/fwd.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtc/epsilon.hpp>

#include <nlohmann/json.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Vec3Hash
{
    size_t operator()(const glm::vec3& v) const
    {
        constexpr float epsilon = EPSILON;

        auto to_grid = [](float value) -> int {
            return static_cast<int>(round(value / epsilon));
        };

        size_t h1 = std::hash<int>{}(to_grid(v.x));
        size_t h2 = std::hash<int>{}(to_grid(v.y));
        size_t h3 = std::hash<int>{}(to_grid(v.z));

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct Vec3Equal
{
    inline bool operator()(const glm::vec3& a, const glm::vec3& b) const
    {
        constexpr float epsilon = EPSILON;

        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};

struct Vec2Hash
{
    size_t operator()(const glm::vec2& v) const
    {
        constexpr float epsilon = EPSILON;

        auto to_grid = [](float value) -> int {
            return static_cast<int>(round(value / epsilon));
        };

        size_t h1 = std::hash<int>{}(to_grid(v.x));
        size_t h2 = std::hash<int>{}(to_grid(v.y));

        return h1 ^ (h2 << 1);
    }
};

struct Vec2Equal
{
    inline bool operator()(const glm::vec2& a, const glm::vec2& b) const
    {
        constexpr float epsilon = EPSILON;

        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};

namespace nlohmann
{
    inline void to_json(json& j, const glm::vec2& v)
    {
        j = { v.x, v.y };
    }

    inline void to_json(json& j, const glm::vec3& v)
    {
        j = { v.x, v.y, v.z };
    }

    inline void to_json(json& j, const glm::vec4& v)
    {
        j = { v.x, v.y, v.z, v.w };
    }

    inline json vertex_to_json(const Vertex& v, bool genTangents, bool calcBitangents, bool positiveHandedness)
    {
        json j = {
            { "position",  v.Position },
            { "texCoord",  v.TexCoord },
            { "normal",    v.Normal }
        };

        if (genTangents) {
            if (calcBitangents)
            {
                j["tangent"] = v.Tangent;
                j["bitangent"] = v.Bitangent;
            }
            else
            {
                j["tangent"] = glm::vec4(
                    v.Tangent,
                    positiveHandedness ? 1.0f : -1.0f
                );
            }
        }

        return j;
    }

    inline json vertex_vector_to_json(const std::vector<Vertex>& vertices, bool genTangents, bool calcBitangents, bool positiveHandedness)
    {
        json j = json::array();

        for (const Vertex& v : vertices)
        {
            j.push_back(vertex_to_json(v, genTangents, calcBitangents, positiveHandedness));
        }

        return j;
    }
}