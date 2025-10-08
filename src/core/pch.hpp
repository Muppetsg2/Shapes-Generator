#pragma once

// STANDARD LIBS
#include <algorithm>
//#include <cctype>
//#include <chrono>
#include <cmath>
//#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
//#include <functional>
//#include <iomanip>
//#include <iostream>
#include <istream>
//#include <map>
//#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
//#include <unordered_map>
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
#define MKDIR(path) _mkdir(path)
#define DIRSEP "\\"
#else
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0777)
#define DIRSEP "/"
#endif

#if __cplusplus > 201703L
#include <concepts>
#else
#include <type_traits>
#endif

// FROM VCPKG
#define FMT_HEADER_ONLY
//#include <fmt/core.h>
#include <fmt/base.h>
//#include <fmt/args.h>
#include <fmt/color.h>
#include <glm/fwd.hpp>
#include <glm/detail/func_geometric.inl>
//#include <glm/gtc/epsilon.hpp>
//#include <glm/gtc/constants.hpp>

// DEFINES
#define EPSILON 1e-6f
#define M_PI_3 1.04719755119659774615	  // pi/3
#define M_SQRT3 1.73205080756887729353	  // sqrt(3)
#define M_SQRT5 2.23606797749978969641    // sqrt(5)
#define M_SQRT1_3 0.57735026918962576451  // 1/sqrt(3)
#define M_SQRT1_5 0.44721359549995793928  // 1/sqrt(5)
#if defined(_WIN32)
#include <corecrt_math_defines.h>
#else
#include <math.h>
#endif