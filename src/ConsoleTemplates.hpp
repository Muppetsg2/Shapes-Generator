#pragma once
#pragma region STD_LIBS
#include <chrono>
#include <functional>
#include <string>
#pragma endregion

#pragma region FMT_LIB
#include <fmt/base.h>
#include <fmt/color.h>
#pragma endregion

#pragma region MY_FILES
#include <Shape.hpp>
#pragma endregion

template<class ShapeType, class... Args>
static std::chrono::duration<double> generateShape(Shape*& out, const Args&... args) {
    
    fmt::print("\n[{}] Start Generating {}...\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    auto start = std::chrono::system_clock::now();

    // Shape Creation
    out = new ShapeType(args...);

    auto end = std::chrono::system_clock::now();

    fmt::print("[{}] {} Generated Successfully!\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    return end - start;
}

template<class T>
static T getShadingType(const std::string& shapeName, std::function<int(int, int)> intInputFunc, std::function<void(int, int)> errorMsgFunc) {
    int dir_choice;
    do {
        fmt::print("\n> Select the {} shading type:\n1) FLAT\n2) SMOOTH\n", shapeName);
        //dir_choice = getIntInput(getInputPrompt(1, 2));
        dir_choice = intInputFunc(1, 2);
        if (dir_choice < 1 || dir_choice > 2) {
            errorMsgFunc(1, 2);
        }
    } while (dir_choice < 1 || dir_choice > 2);

    return static_cast<T>(dir_choice - 1);
}