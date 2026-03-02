#pragma once

// STANDARD LIBS
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

// PLATFORM DEPENDANT
#if defined(_WIN32)
#define DIRSEP '\\'
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <direct.h>
#include <shellapi.h>
#define MKDIR(path) _mkdir(path)
#else
#define DIRSEP '/'
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

// FROM CPM
#include <fmt/base.h>
#include <fmt/color.h>