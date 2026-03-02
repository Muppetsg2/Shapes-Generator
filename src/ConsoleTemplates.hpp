#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <ratio>
#pragma endregion

#pragma region FMT_LIB
#include <fmt/base.h>
#include <fmt/color.h>
#pragma endregion

#pragma region MY_FILES_CORE_LIB
#include <Shape.hpp>
#pragma endregion

template<class ShapeType, class... Args>
static std::chrono::duration<double, std::milli> generateShape(Shape*& out, const ShapeConfig& config, const Args&... args) {
    
    fmt::print("\n[{}] Start Generating {}...\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    auto start = std::chrono::high_resolution_clock::now();

    // Shape Creation
    out = new ShapeType(config, args...);

    auto end = std::chrono::high_resolution_clock::now();

    fmt::print("[{}] {} Generated Successfully!\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start);
}