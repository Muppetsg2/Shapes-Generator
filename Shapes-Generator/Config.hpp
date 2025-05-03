#pragma once

struct Config {
	std::string saveDir;
	std::string fileName;
};

static Config load_config(const std::string& exeDirPath)
{
    const std::string configFilePath = exeDirPath + "\\shapes.config";
    std::ifstream inFile(configFilePath);
    Config config;
    config.saveDir = exeDirPath + "\\";
    config.fileName = "shape";

    if (inFile) {
        std::string line;
        while (std::getline(inFile, line)) {
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (key == "saveDir") config.saveDir = value;
            else if (key == "fileName") config.fileName = value;
        }
        inFile.close();
    }
    else {
        std::ofstream outFile(configFilePath);
        if (outFile) {
            outFile << "saveDir: " << config.saveDir << "\n";
            outFile << "fileName: " << config.fileName << "\n";
            outFile.close();
        }
        else {
            fmt::print("[{}] Warning: Could not create shapes.config. Using defaults.\n",
                fmt::styled("WARN", fmt::fg(fmt::color::yellow)));
        }
    }

    return config;
}