#pragma once

// STANDARD LIBS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// PLATFORM DEPENDANT
#if defined(_WIN32)
#define DIRSEP "\\"
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
#define DIRSEP "/"
#include <termios.h>
#include <unistd.h>
#endif

// FROM VCPKG
#define FMT_HEADER_ONLY
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>

// CORE_LIB
#include <Cone.hpp>
#include <Config.hpp>
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