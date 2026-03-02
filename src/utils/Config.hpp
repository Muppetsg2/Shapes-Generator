#pragma once
#include <string>

namespace utils {
	struct Config {
		std::string saveDir;
		std::string fileName;
		bool genTangents;
		bool calcBitangents;
		bool tangentHandednessPositive;
		bool openDirOnSave;
	};

	Config& get_config(const std::string& exeDirPath = ".");

	std::string get_resolved_file_name(const Config& cfg, const std::string& typeName);

	Config& update_config(Config& cfg, Config newData, const std::string& exeDirPath = ".");

	namespace internal {
		void save_config_file(Config& cfg, const std::string& configFilePath);

		std::string get_config_path(const std::string& exeDirPath = ".");
	}
}