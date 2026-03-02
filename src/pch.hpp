#pragma once

// STANDARD LIBS
#include <chrono>
#include <cstdlib>
#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ratio>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

// PLATFORM DEPENDANT
#if defined(_WIN32)
#define DIRSEP '\\'
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <consoleapi.h>
#include <processenv.h>
#include <conio.h>
#else
#define DIRSEP '/'
#include <termios.h>
#include <unistd.h>
#endif

// FROM CPM
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

// CORE_LIB
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

// TUI_LIB
#include <EditConfigView.hpp>
#include <GenerateView.hpp>
#include <SaveFormatView.hpp>
#include <SaveResultView.hpp>
#include <SaveView.hpp>
#include <ShapeSelectView.hpp>
#include <ShapesParametersViews.hpp>

// UTILS_LIB
#include <Config.hpp>
#include <StringHelpers.hpp>
#include <SystemFunctions.hpp>