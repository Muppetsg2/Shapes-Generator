#pragma once

// STANDARD LIBS
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unordered_map>
#include <utility>
#include <vector>

// PLATFORM DEPENDANT
#if defined(_WIN32)
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <direct.h>
#include <shellapi.h>
#define MKDIR(path) _mkdir(path)
#define DIRSEP '\\'
#else
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0777)
#define DIRSEP '/'
#endif

#if __cplusplus > 201703L
#include <concepts>
#else
#include <type_traits>
#endif

// FROM CPM
#define FMT_HEADER_ONLY
#include <fmt/args.h>
#include <fmt/base.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>
#include <nlohmann/json.hpp>

#include "Constants.hpp"
#include "BitMathOperators.hpp"