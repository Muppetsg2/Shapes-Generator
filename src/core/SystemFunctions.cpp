#include "pch.hpp"
#include "SystemFunctions.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(_WIN32)
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <direct.h>
#else
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <unistd.h>
#endif

static bool check_directory(const char* path)
{
	struct stat info;
	return stat(path, &info) == 0 && (info.st_mode & S_IFDIR);
}

static bool create_directory(const char* path)
{
	if (!path || *path == '\0') return false;

	std::string dir(path);
	if (dir.back() == '/' || dir.back() == '\\') {
		dir.pop_back();
	}

	if (MKDIR(dir.c_str()) == 0) return true; // Try to create directory

	// Create parent directory first
	size_t pos = dir.find_last_of("/\\");
	if (pos != std::string::npos) {
		std::string parent = dir.substr(0, pos);
		if (!create_directory(parent.c_str())) {
			return false;
		}
	}

	return MKDIR(dir.c_str()) == 0; // Try again after creating parent
}

static inline std::string get_executable_path()
{
    char buffer[4096] = {};
    uint32_t size = sizeof(buffer);

#if defined(_WIN32)
    DWORD len = GetModuleFileNameA(NULL, buffer, size);
    if (len == 0 || len == size) {
        fprintf(stderr, "Couldn't fetch exe file directory");
        exit(EXIT_FAILURE);
    }
#elif defined(__APPLE__)
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        fprintf(stderr, "Couldn't fetch exe file directory");
        exit(EXIT_FAILURE);
    }
#elif defined(__linux__)
    ssize_t len = readlink("/proc/self/exe", buffer, size - 1);
    if (len == -1) {
        fprintf(stderr, "Couldn't fetch exe file directory");
        exit(EXIT_FAILURE);
    }
    buffer[len] = '\0';
#else
    fprintf(stderr, "Unknown OS");
    exit(EXIT_FAILURE);
#endif

    return std::filesystem::absolute(std::filesystem::path(buffer)).parent_path().string();
}