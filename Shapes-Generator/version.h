#pragma once

#define SHAPES_GENERATOR_VER_MAJOR 1
#define SHAPES_GENERATOR_VER_MINOR 1
#define SHAPES_GENERATOR_VER_PATCH 1

#define SHAPES_GENERATOR_TO_STRING_VERSION(major, minor, patch) std::format("{}.{}.{}", major, minor, patch)
#define SHAPES_GENERATOR_VERSION_STR SHAPES_GENERATOR_TO_STRING_VERSION(SHAPES_GENERATOR_VER_MAJOR, SHAPES_GENERATOR_VER_MINOR, SHAPES_GENERATOR_VER_PATCH)