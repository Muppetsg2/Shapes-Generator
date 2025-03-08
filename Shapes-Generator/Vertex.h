#pragma once

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        constexpr float epsilon = 1e-6f;

        auto to_grid = [](float value) -> int {
            return static_cast<int>(std::round(value / epsilon));
        };

        std::size_t h1 = std::hash<int>{}(to_grid(v.x));
        std::size_t h2 = std::hash<int>{}(to_grid(v.y));
        std::size_t h3 = std::hash<int>{}(to_grid(v.z));

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct Vec3Equal {
    constexpr static float epsilon = 1e-6f;

    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};

struct Vec2Hash {
    std::size_t operator()(const glm::vec2& v) const {
        constexpr float epsilon = 1e-6f;

        auto to_grid = [](float value) -> int {
            return static_cast<int>(std::round(value / epsilon));
        };

        std::size_t h1 = std::hash<int>{}(to_grid(v.x));
        std::size_t h2 = std::hash<int>{}(to_grid(v.y));

        return h1 ^ (h2 << 1);
    }
};

struct Vec2Equal {
    constexpr static float epsilon = 1e-6f;

    bool operator()(const glm::vec2& a, const glm::vec2& b) const {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }
};