#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIB
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>
#pragma endregion

#pragma region FMT_LIB
#include <fmt/base.h>
#include <fmt/color.h>
#pragma endregion

#pragma region MY_FILES
#include "Config.hpp"
#include "StringHelpers.hpp"
#pragma endregion

utils::Config& utils::get_config(const std::string& exeDirPath)
{
    static bool init = false;
    static utils::Config config;

    if (init) {
        return config;
    }

    const std::string configFilePath = utils::internal::get_config_path(exeDirPath);
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
                config.genTangents = utils::parse_bool(value);
                hasGenTangents = true;
            }
            else if (key == "calculateBitangents") {
                config.calcBitangents = utils::parse_bool(value);
                hasCalcBitangents = true;
            }
            else if (key == "tangentHandednessPositive") {
                config.tangentHandednessPositive = utils::parse_bool(value);
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
                config.openDirOnSave = utils::parse_bool(value);
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
        utils::internal::save_config_file(config, configFilePath);
    }

    return config;
}

std::string utils::get_resolved_file_name(const utils::Config& cfg, const std::string& typeName) {
    std::string result = cfg.fileName;

    static const std::string typePlaceholder = "${TYPE}";
    static const std::string typeToken = "___TYPE_TOKEN___";

    utils::replace_all(result, typePlaceholder, typeToken);

    // Actual Time
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    if (!result.empty()) {
        oss << std::put_time(::std::localtime(&t), result.c_str());
    }
    else {
        oss << std::put_time(::std::localtime(&t), "___TYPE_TOKEN___-%H-%M-%S");
    }
    result = oss.str();

    utils::replace_all(result, typeToken, typeName);

    return result;
}

utils::Config& utils::update_config(utils::Config& cfg, utils::Config newData, const std::string& exeDirPath)
{
    cfg = newData;
    const std::string configFilePath = utils::internal::get_config_path(exeDirPath);
    utils::internal::save_config_file(cfg, configFilePath);
    return cfg;
}

void utils::internal::save_config_file(utils::Config& cfg, const std::string& configFilePath)
{
    std::ofstream outFile(configFilePath, std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "generateTangents: " << (cfg.genTangents ? "true" : "false") << "\n";
        outFile << "calculateBitangents: " << (cfg.calcBitangents ? "true" : "false") << "\n";
        outFile << "tangentHandednessPositive: " << (cfg.tangentHandednessPositive ? "true" : "false") << "\n";
        outFile << "saveDir: " << cfg.saveDir << "\n";
        outFile << "fileName: " << cfg.fileName << "\n";
        outFile << "openDirOnSave: " << (cfg.openDirOnSave ? "true" : "false") << "\n";
        outFile.close();
    }
    else {
        fmt::print("[{}] Warning: Could not create shapes.config. Using defaults.\n",
            fmt::styled("WARN", fmt::fg(fmt::color::yellow)));
    }
}

std::string utils::internal::get_config_path(const std::string& exeDirPath)
{
    return exeDirPath + DIRSEP + "shapes.config";
}