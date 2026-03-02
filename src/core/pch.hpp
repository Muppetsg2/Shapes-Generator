#pragma once

// STANDARD LIBS
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// C++ DEPENDANT
#if __cplusplus > 201703L
#include <concepts>
#endif

// PLATFORM DEPENDANT
#if defined(_WIN32)
#include <corecrt_math_defines.h>
#endif

// FROM CPM
#include <fmt/args.h>
#include <fmt/base.h>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>

#include <nlohmann/json.hpp>

// MY FILES
#include "BitMathOperators.hpp"
#include "Constants.hpp"