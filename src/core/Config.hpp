#pragma once
#include <string>

namespace config {
	struct Config {
		std::string saveDir;
		std::string fileName;
		bool genTangents;
	};

	Config& get_config(const std::string& exeDirPath = ".");

	std::string get_resolved_file_name(const Config& cfg, const std::string& typeName);
}