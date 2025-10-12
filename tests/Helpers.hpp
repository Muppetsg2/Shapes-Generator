#pragma once
#include <cmath>

static inline bool epsilon_equal(float a, float b, float eps) { return fabsf(a - b) < eps; }

struct Vec3EqualForTests {
	inline constexpr bool operator()(const glm::vec3& a, const glm::vec3& b, const float eps) const
	{
		return glm::all(glm::epsilonEqual(a, b, eps));
	}
};

struct Vec2EqualForTests {
	inline constexpr bool operator()(const glm::vec2& a, const glm::vec2& b, const float eps) const
	{
		return glm::all(glm::epsilonEqual(a, b, eps));
	}
};

static void CheckVec3Equal(const glm::vec3& value, const glm::vec3& expected, float eps, const std::string& label) {
	INFO("with values:\n\t" << label << " := " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec3EqualForTests{}(value, expected, eps));
}

static void CheckVec2Equal(const glm::vec2& value, const glm::vec2& expected, float eps, const std::string& label) {
	INFO("with values:\n\t" << label << " := " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec2EqualForTests{}(value, expected, eps));
}