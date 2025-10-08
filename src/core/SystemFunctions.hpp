#pragma once
#include <string>

static bool check_directory(const char* path);

static bool create_directory(const char* path);

static inline std::string get_executable_path();