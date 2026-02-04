#include "pch.hpp"
#include "Config.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>

#include <fmt/base.h>
#include <fmt/color.h>

config::Config& config::get_config(const std::string& exeDirPath)
{
    static bool init = false;
    static config::Config config;

    if (init) {
        return config;
    }

    const std::string configFilePath = exeDirPath + DIRSEP + "shapes.config";
    std::ifstream inFile(configFilePath);
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;
    config.saveDir = exeDirPath + DIRSEP;
    config.fileName = "${TYPE}-%H-%M-%S";
    config.openDirOnSave = true;

    init = true;

    bool hasGenTangents = false;
    bool hasCalcBitangents = false;
    bool hasTangentHandedness = false;
    bool hasSaveDir = false;
    bool hasFileName = false;
    bool hasOpenDirOnSave = false;

    auto parseBool = [](const std::string& str) -> bool {
        std::string value = str;
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return (value == "true" || value == "1");
    };

    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (key == "generateTangents") {
                config.genTangents = parseBool(value);
                hasGenTangents = true;
            }
            else if (key == "calculateBitangents") {
                config.calcBitangents = parseBool(value);
                hasCalcBitangents = true;
            }
            else if (key == "tangentHandednessPositive") {
                config.tangentHandednessPositive = parseBool(value);
                hasTangentHandedness = true;
            }
            else if (key == "saveDir") {
                config.saveDir = value;
                hasSaveDir = true;
            }
            else if (key == "fileName") {
                config.fileName = value;
                hasFileName = true;
            }
            else if (key == "openDirOnSave") {
                config.openDirOnSave = parseBool(value);
                hasOpenDirOnSave = true;
            }
        }
        inFile.close();

        if (!hasGenTangents || !hasCalcBitangents || !hasTangentHandedness || !hasSaveDir || !hasFileName || !hasOpenDirOnSave) {
            std::ofstream outFile(configFilePath, std::ios::app);
            if (outFile.is_open()) {
                if (!hasGenTangents)
                    outFile << "\ngenerateTangents: " << (config.genTangents ? "true" : "false") << "\n";
                if (!hasCalcBitangents)
                    outFile << "\ncalculateBitangents: " << (config.calcBitangents ? "true" : "false") << "\n";
                if (!hasTangentHandedness)
                    outFile << "\ntangentHandednessPositive: " << (config.tangentHandednessPositive ? "true" : "false") << "\n";
                if (!hasSaveDir)
                    outFile << "\nsaveDir: " << config.saveDir << "\n";
                if (!hasFileName)
                    outFile << "\nfileName: " << config.fileName << "\n";
                if (!hasOpenDirOnSave)
                    outFile << "\nopenDirOnSave: " << (config.openDirOnSave ? "true" : "false") << "\n";
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
        if (outFile.is_open()) {
            outFile << "generateTangents: " << (config.genTangents ? "true" : "false") << "\n";
            outFile << "calculateBitangents: " << (config.calcBitangents ? "true" : "false") << "\n";
            outFile << "tangentHandednessPositive: " << (config.tangentHandednessPositive ? "true" : "false") << "\n";
            outFile << "saveDir: " << config.saveDir << "\n";
            outFile << "fileName: " << config.fileName << "\n";
            outFile << "openDirOnSave: " << (config.openDirOnSave ? "true" : "false") << "\n";
            outFile.close();
        }
        else {
            fmt::print("[{}] Warning: Could not create shapes.config. Using defaults.\n",
                fmt::styled("WARN", fmt::fg(fmt::color::yellow)));
        }
    }

    return config;
}

std::string config::get_resolved_file_name(const config::Config& cfg, const std::string& typeName) {
    std::string result = cfg.fileName;

    static const std::string typePlaceholder = "${TYPE}";
    static const std::string typeToken = "___TYPE_TOKEN___";

    auto replace_all = [](std::string& str, const std::string& from, const std::string& to) {
        size_t pos = 0;
            while ((pos = str.find(from, pos)) != std::string::npos) {
                str.replace(pos, from.length(), to);
                pos += to.length();
            }
        };

    replace_all(result, typePlaceholder, typeToken);

    // Actual Time
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    ::std::ostringstream oss;
    if (!result.empty()) {
        oss << ::std::put_time(::std::localtime(&t), result.c_str());
    }
    else {
        oss << ::std::put_time(::std::localtime(&t), "___TYPE_TOKEN___-%H-%M-%S");
    }
    result = oss.str();

    replace_all(result, typeToken, typeName);

    return result;
}