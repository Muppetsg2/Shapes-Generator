#pragma once

// DEFINES
#define TEST_EPSILON 1e-5f

// STANDARD LIBS
#include <cmath>
#include <vector>

// FROM CPM
#include <catch2/catch_approx.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/fwd.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vector_relational.hpp>

// CORE_LIB
#include <BitMathOperators.hpp>
#include <Cone.hpp>
#include <Cube.hpp>
#include <Cylinder.hpp>
#include <Hexagon.hpp>
#include <IcoSphere.hpp>
#include <Plane.hpp>
#include <Pyramid.hpp>
#include <Shape.hpp>
#include <Sphere.hpp>
#include <Tetrahedron.hpp>
#include <Torus.hpp>
#include <Vertex.hpp>