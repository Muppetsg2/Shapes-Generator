#pragma once
#include <string>

namespace system_functions {
	bool check_directory(const char* path);

	bool create_directory(const char* path);

	std::string get_executable_path();
}