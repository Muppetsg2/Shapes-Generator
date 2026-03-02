#pragma once

#pragma region STD_LIBS
#include <functional>
#include <string>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES_UTILS_LIB
#include <Config.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

namespace tui {
    class EditConfigView : public View {
    public:
        EditConfigView(utils::Config& config, const std::string exeDirPath, std::function<void()> onBack);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    private:
        bool CheckIfChanged();

        utils::Config& _config;
        utils::Config _currentConfig;

        std::string _exeDirPath;

        std::function<void()> _onBack;

        ftxui::Component _saveDirInput;
        ftxui::Component _fileNameInput;
        ftxui::Component _genTangentsCheckbox;
        ftxui::Component _calcBitangentsCheckbox;
        ftxui::Component _handednessCheckbox;
        ftxui::Component _openDirCheckbox;
        ftxui::Component _saveButton;
        ftxui::Component _backButton;

        ftxui::Component _container;
        ftxui::Component _renderer;

        bool _showSavedMessage = false;
        int _messageTimer = 0;
    };
}