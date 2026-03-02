#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <functional>
#include <string>
#include <type_traits>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#pragma endregion

#pragma region MY_FILES_UTILS_LIB
#include <Config.hpp>
#pragma endregion

#pragma region MY_FILES
#include "EditConfigView.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

tui::EditConfigView::EditConfigView(utils::Config& config, const std::string exeDirPath, std::function<void()> onBack)
    : _config(config), _onBack(std::move(onBack)), _exeDirPath(exeDirPath)
{
    _currentConfig = config;

    _saveDirInput = Input(&_currentConfig.saveDir, "path/to/dir");
    _fileNameInput = Input(&_currentConfig.fileName, "name");

    _genTangentsCheckbox = Checkbox("Generate Tangents", &_currentConfig.genTangents);
    _calcBitangentsCheckbox = Checkbox("Calculate Bitangents", &_currentConfig.calcBitangents);
    _handednessCheckbox = Checkbox("Positive Handedness (w = 1.0)", &_currentConfig.tangentHandednessPositive);
    _openDirCheckbox = Checkbox("Open folder after save", &_currentConfig.openDirOnSave);

    _saveButton = Button(" SAVE SETTINGS ", [this] {
        if (CheckIfChanged())
        {
            utils::update_config(_config, _currentConfig, _exeDirPath);
            _showSavedMessage = true;
            _messageTimer = 80;
        }
    }, ButtonOption::Simple());

    _backButton = Button("Back", _onBack);

    _container = Container::Vertical({
        _backButton,
        _saveDirInput,
        _fileNameInput,
        _genTangentsCheckbox,
        _calcBitangentsCheckbox,
        _handednessCheckbox,
        _openDirCheckbox,
        _saveButton
    });

    _renderer = Renderer(_container, [this] {
        bool isChanged = CheckIfChanged();

        Element saveStatus = vbox({
            _saveButton->Render() | color(isChanged ? Color::Yellow : Color::GrayDark) | hcenter
        });

        if (_showSavedMessage) {
            saveStatus = vbox({
                text(" SUCCESS: Configuration saved! ") | color(Color::Green) | hcenter,
                _saveButton->Render() | color(isChanged ? Color::Yellow : Color::GrayDark) | hcenter
            });
            if (--_messageTimer <= 0) _showSavedMessage = false;
        }
        else if (isChanged) {
            saveStatus = vbox({
                text(" * Unsaved changes detected ") | color(Color::Yellow) | italic | hcenter,
                _saveButton->Render() | color(isChanged ? Color::Yellow : Color::GrayDark) | hcenter
            });
        }

        Elements content = {
            hbox(text(" Save Directory: ") | size(WIDTH, EQUAL, 20), _saveDirInput->Render() | focus) | size(WIDTH, GREATER_THAN, 40),
            hbox(text(" Default Name:   ") | size(WIDTH, EQUAL, 20), _fileNameInput->Render()) | size(WIDTH, GREATER_THAN, 40),
            separator(),
            _genTangentsCheckbox->Render(),
            _calcBitangentsCheckbox->Render(),
            _handednessCheckbox->Render(),
            _openDirCheckbox->Render(),
            separator(),
            saveStatus
        };

        return vbox({
            filler(),
            vbox({
                hbox({
                    _backButton->Render(),
                    filler(),
                    text(" CONFIGURATION ") | bold | color(Color::Cyan1) | center,
                    filler(),
                    text("    ")
                }),
                separator(),
                vbox(std::move(content)) | frame | size(HEIGHT, LESS_THAN, 15)
            }) | borderRounded | center | size(WIDTH, GREATER_THAN, 70),
            filler(),
            ControlsInfo({
                KeyInfo { "Arrows", "Navigate" },
                KeyInfo { "Enter", "Toggle/Edit" },
                KeyInfo { "ESC", "Back" }
            })
        }) | borderRounded;
    });
}

Component tui::EditConfigView::GetComponent()
{
    return _renderer;
}

bool tui::EditConfigView::OnEvent(Event event)
{
    if (event == Event::Escape) {
        _onBack();
        return true;
    }
    return false;
}

bool tui::EditConfigView::CheckIfChanged()
{
    return _currentConfig.saveDir                   != _config.saveDir        ||
           _currentConfig.fileName                  != _config.fileName       ||
           _currentConfig.genTangents               != _config.genTangents    ||
           _currentConfig.openDirOnSave             != _config.openDirOnSave  ||
           _currentConfig.calcBitangents            != _config.calcBitangents ||
           _currentConfig.tangentHandednessPositive != _config.tangentHandednessPositive;
}