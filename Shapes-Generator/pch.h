#pragma once

// STANDARD LIBS
#include <cctype>
#include <chrono>
#include <cmath>
#include <conio.h>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// FROM VCPKG
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

// DEFINES
#define EPSILON 1e-6f
#define M_PI_3 1.04719755119659774615	  // pi/3
#define M_SQRT3 1.73205080756887729353	  // sqrt(3)
#define M_SQRT5 2.23606797749978969641    // sqrt(5)
#define M_SQRT1_3 0.57735026918962576451  // 1/sqrt(3)
#define M_SQRT1_5 0.44721359549995793928  // 1/sqrt(5)
#include <corecrt_math_defines.h>

// VERSION
#include "version.h"