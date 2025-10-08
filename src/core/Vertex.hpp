#pragma once
#include "Constants.hpp"

#include <cmath>
#include <functional>

#include <glm/fwd.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtc/epsilon.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Vec3Hash {
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

struct Vec3Equal {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const
    {
        constexpr float epsilon = EPSILON;

        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};

struct Vec2Hash {
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

struct Vec2Equal {
    bool operator()(const glm::vec2& a, const glm::vec2& b) const
    {
        constexpr float epsilon = EPSILON;

        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};