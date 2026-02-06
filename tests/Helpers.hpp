#pragma once
#include <cmath>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include <glm/fwd.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

static inline bool epsilon_equal(float a, float b, float eps) { return fabsf(a - b) < eps; }

struct Vec3EqualForTests {
	inline bool operator()(const glm::vec3& a, const glm::vec3& b, const float eps) const
	{
		return glm::all(glm::epsilonEqual(a, b, eps));
	}
};

struct Vec2EqualForTests {
	inline bool operator()(const glm::vec2& a, const glm::vec2& b, const float eps) const
	{
		return glm::all(glm::epsilonEqual(a, b, eps));
	}
};

static void CheckVec3Equal(const glm::vec3& value, const glm::vec3& expected, float eps, const std::string& label) {
	INFO("with values:\n\t" << label << " := " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec3EqualForTests{}(value, expected, eps));
}

static void CheckVec3Equal(const glm::vec3& value, const glm::vec3& expected, float eps, const std::string& label, const size_t idx) {
	INFO("with values:\n\t" << label << "[" << idx << "] : = " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec3EqualForTests{}(value, expected, eps));
}

static void CheckVec2Equal(const glm::vec2& value, const glm::vec2& expected, float eps, const std::string& label) {
	INFO("with values:\n\t" << label << " := " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec2EqualForTests{}(value, expected, eps));
}

static void CheckVec2Equal(const glm::vec2& value, const glm::vec2& expected, float eps, const std::string& label, const size_t idx) {
	INFO("with values:\n\t" << label << "[" << idx << "] : = " << glm::to_string(value)
		<< "\n\texpected := " << glm::to_string(expected));
	REQUIRE(Vec2EqualForTests{}(value, expected, eps));
}

static void CheckInRange(const float& value, float min, float max, float eps, const std::string& label = "value") {
	INFO("\n" << label << " : = " << std::to_string(value)
		<< "\nrange := " << std::to_string(min - eps) << " <= " << label << " <= " << std::to_string(max + eps));
	REQUIRE((value >= min - eps && value <= max + eps) == true);
}