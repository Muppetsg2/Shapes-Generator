#pragma once
#include <string>

struct Config {
    bool genTangents;
	std::string saveDir;
	std::string fileName;
};

static Config& get_config(const std::string& exeDirPath = ".");