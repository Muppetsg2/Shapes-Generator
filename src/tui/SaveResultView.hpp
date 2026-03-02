#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <functional>
#include <string>
#include <ratio>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

namespace tui {
    class SaveResultView : public View {
    public:
        SaveResultView(int& fileSavedStatus, std::string exePath, std::string& filePath, std::chrono::duration<double, std::milli>& saveTime, std::function<void()> onNext, std::function<void()> onExit);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    private:
        int& _status;
        std::string& _path;
        std::string _exePath;
        std::chrono::duration<double, std::milli>& _time;

        std::function<void()> _onNext;
        std::function<void()> _onExit;

        bool _wasCopied = false;

        ftxui::Component _copyButton;
        ftxui::Component _nextGenButton;
        ftxui::Component _exitButton;

        ftxui::Component _container;
        ftxui::Component _renderer;
    };
}