#pragma once

struct Config {
    bool genTangents;
	std::string saveDir;
	std::string fileName;
};

static Config& get_config(const std::string& exeDirPath = ".")
{
    static bool init = false;
    static Config config;

    if (init) {
        return config;
    }

    const std::string configFilePath = exeDirPath + DIRSEP + "shapes.config";
    std::ifstream inFile(configFilePath);
    config.genTangents = true;
    config.saveDir = exeDirPath + DIRSEP;
    config.fileName = "shape";

    init = true;

    bool hasGenTangents = false;
    bool hasSaveDir = false;
    bool hasFileName = false;

    if (inFile) {
        std::string line;
        while (std::getline(inFile, line)) {
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (key == "generateTangents") {
                std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) 
                { 
                    return std::tolower(c); 
                });
                config.genTangents = (value == "true" || value == "1");
                hasGenTangents = true;
            }
            else if (key == "saveDir") {
                config.saveDir = value;
                hasSaveDir = true;
            }
            else if (key == "fileName") {
                config.fileName = value;
                hasFileName = true;
            }
        }
        inFile.close();

        if (!hasGenTangents || !hasSaveDir || !hasFileName) {
            std::ofstream outFile(configFilePath, std::ios::app);
            if (outFile) {
                if (!hasGenTangents)
                    outFile << "generateTangents: " << (config.genTangents ? "true" : "false") << "\n";
                if (!hasSaveDir)
                    outFile << "saveDir: " << config.saveDir << "\n";
                if (!hasFileName)
                    outFile << "fileName: " << config.fileName << "\n";
                outFile.close();
            }
            else {
                fmt::print("[{}] Warning: Could not update shapes.config with default values.\n",
                    fmt::styled("WARN", fmt::fg(fmt::color::yellow)));
            }
        }
    }
    else {
        std::ofstream outFile(configFilePath);
        if (outFile) {
            outFile << "generateTangents: " << (config.genTangents ? "true" : "false") << "\n";
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