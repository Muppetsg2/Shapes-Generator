//
//   ____  _                              ____                           _             
//  / ___|| |__   __ _ _ __   ___  ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __ 
//  \___ \| '_ \ / _` | '_ \ / _ \/ __| | |  _ / _ \ '_ \ / _ \ '__/ _` | __/ _ \| '__|
//   ___) | | | | (_| | |_) |  __/\__ \ | |_| |  __/ | | |  __/ | | (_| | || (_) | |   
//  |____/|_| |_|\__,_| .__/ \___||___/  \____|\___|_| |_|\___|_|  \__,_|\__\___/|_|   
//                    |_|                                                                
//
// Version: 2.0.0
// Author: Marceli Antosik (Muppetsg2)
// Last Update: 04.03.2026

#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region MY_FILES
#include "TuiApp.hpp"
#include "CliApp.hpp"
#pragma endregion

int main(int argc, char** argv) {
#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    std::string exe_dir_path = utils::get_executable_path();
    utils::Config& _ = utils::get_config(exe_dir_path);
    
    bool isCli = argc > 1 && (std::string(argv[1]) == std::string("-c") || std::string(argv[1]) == std::string("--cli"));

    if (argc > 1 && !isCli)
    {
        fmt::print("\n[{}] Error: Unknown argument '{}'!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)), std::string(argv[1]));
        return EXIT_FAILURE;
    }

    if (isCli)
    {
        return cli::cliApp();
    }
    else
    {
        tui::App App;
        App.Run();
    }

    return EXIT_SUCCESS;
}