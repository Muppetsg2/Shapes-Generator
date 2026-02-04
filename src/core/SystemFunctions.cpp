#include "pch.hpp"
#include "SystemFunctions.hpp"

#include <cstdint>
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
#include <shellapi.h>
#else
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#include <unistd.h>
#endif

namespace fs = std::filesystem;

bool system_functions::check_directory(const char* path)
{
	struct stat info;
	return stat(path, &info) == 0 && (info.st_mode & S_IFDIR);
}

bool system_functions::create_directory(const char* path)
{
	if (!path || *path == '\0') return false;

	std::string dir(path);
	if (dir.back() == DIRSEP) {
		dir.pop_back();
	}

	if (MKDIR(dir.c_str()) == 0) return true; // Try to create directory

	// Create parent directory first
	size_t pos = dir.find_last_of(DIRSEP);
	if (pos != std::string::npos) {
		std::string parent = dir.substr(0, pos);
		if (!create_directory(parent.c_str())) {
			return false;
		}
	}

	return MKDIR(dir.c_str()) == 0; // Try again after creating parent
}

std::string system_functions::get_executable_path()
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

    return fs::absolute(fs::path(buffer)).parent_path().string();
}

#if defined(__linux__)
static bool command_linux_exists(const std::string& cmd)
{
    std::string check = "command -v " + cmd + " >/dev/null 2>&1";
    return (system(check.c_str()) == 0);
}
#endif

bool system_functions::open_in_file_explorer(const char* path)
{
    fs::path absPath = fs::absolute(path);

#if defined(_WIN32)
    std::string parameters = "/select,\"" + absPath.string() + "\"";

    SHELLEXECUTEINFO sei{};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = "explorer.exe";
    sei.lpParameters = parameters.c_str();
    sei.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteEx(&sei))
    {
        DWORD error = GetLastError();
        return false;
    }
    return true;
#elif defined(__APPLE__)
    std::string command = "open -R \"" + absPath.string() + "\"";
#elif defined(__linux__)
    // Linux / Unix
    std::string explorer;

    if      (command_linux_exists("xdg-open"))    explorer = "xdg-open";
    else if (command_linux_exists("gnome-open"))  explorer = "gnome-open";
    else if (command_linux_exists("kde-open"))    explorer = "kde-open";
    else if (command_linux_exists("nautilus"))    explorer = "nautilus";
    else if (command_linux_exists("dolphin"))     explorer = "dolphin";
    else if (command_linux_exists("thunar"))      explorer = "thunar";
    else if (command_linux_exists("pcmanfm"))     explorer = "pcmanfm";
    else {
        fprintf(stderr, "Couldn't recognize file explorer.\n");
        return false;
    }

    std::string command;
    if (fs::is_directory(absPath))
        command = explorer + " \"" + absPath.string() + "\" &";
    else
        command = explorer + " \"" + absPath.parent_path().string() + "\" &";
#else
    fprintf(stderr, "Couldn't recognize OS.\n");
    return false;
#endif

#if defined(__APPLE__) || defined(__linux__)
    return (system(command.c_str()) == 0);
#endif
}