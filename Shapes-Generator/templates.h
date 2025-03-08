#pragma once

template<class ShapeType, class... Args>
std::chrono::duration<double> generateShape(Shape*& out, const Args&... args) {
    
    fmt::print("\n[{}] Start Generating {}...\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    auto start = std::chrono::system_clock::now();

    // Shape Creation
    out = new ShapeType(args...);

    auto end = std::chrono::system_clock::now();

    fmt::print("[{}] {} Generated Successfully!\n", fmt::styled("OK", fmt::fg(fmt::color::green)), ShapeType::getClassName());

    return end - start;
}