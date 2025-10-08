#pragma once
#include <string>

namespace config {
	struct Config {
		bool genTangents;
		std::string saveDir;
		std::string fileName;
	};

	Config& get_config(const std::string& exeDirPath = ".");
}