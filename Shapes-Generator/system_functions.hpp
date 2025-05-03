#pragma once

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

static std::string get_executable_path() {
#if defined(_WIN32)
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path().string();

#elif defined(__APPLE__)
	char buffer[4096];
	uint32_t size = sizeof(buffer);
	if (_NSGetExecutablePath(buffer, &size) == 0) {
		return std::filesystem::weakly_canonical(buffer).parent_path().string();
	}
	else {
		printf("Error: _NSGetExecutablePath(): Buffer too small for executable path");
		exit(EXIT_FAILURE);
	}

#elif defined(__linux__)
	char buffer[4096];
	ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
	if (count != -1) {
		return std::filesystem::weakly_canonical(std::string(buffer, count)).parent_path().string();
	}
	else {
		printf("Error: readlink(): Cannot read /proc/self/exe");
		exit(EXIT_FAILURE);
	}

#else
#error "Unsupported platform"
#endif
	return "";
}