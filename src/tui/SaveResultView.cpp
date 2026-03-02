#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <chrono>
#include <functional>
#include <string>
#include <ratio>
#include <type_traits>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#pragma endregion

#pragma region CLIP_LIB
#include <clip.h>
#pragma endregion

#pragma region MY_FILES
#include "SaveResultView.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

tui::SaveResultView::SaveResultView(int& fileSavedStatus, std::string exePath, std::string& filePath, std::chrono::duration<double, std::milli>& saveTime, std::function<void()> onNext, std::function<void()> onExit)
    : _status(fileSavedStatus), _exePath(exePath), _path(filePath), _time(saveTime), _onNext(std::move(onNext)), _onExit(std::move(onExit))
{
    _copyButton = Button("Copy", [this] { clip::set_text(_path); _wasCopied = true; }, ButtonOption::Ascii());

    ButtonOption b_options;
    b_options.transform = [](const EntryState& s) {
        auto element = vbox({ text(s.label) | hcenter }) | borderLight;
        if (s.focused) {
            element |= inverted;
        }
        return element;
    };

    _nextGenButton = Button("Generate Next", [this] { _wasCopied = false; _onNext(); }, b_options);
    _exitButton = Button("Exit App", _onExit, b_options);

    _container = Container::Vertical({
        _copyButton,
        Container::Horizontal({
            _nextGenButton,
            _exitButton
        })
    });

    _renderer = Renderer(_container, [this] {
        bool isSuccess = (_status == 0 || _status == 1);
        auto headerColor = isSuccess ? Color::GreenLight : Color::RedLight;

        Elements infoLines;
        if (isSuccess) {
            std::string pathTxt = std::string(".") + DIRSEP + std::filesystem::relative(_path, _exePath).string();

            infoLines.push_back(hbox({
                text(" Path: ") | bold | vcenter,
                text(pathTxt) | color(Color::CyanLight) | vcenter,
                filler(),
                text(_wasCopied ? " [Done!] " : "          ") | color(_wasCopied ? Color::Green : Color::White) | vcenter,
                _copyButton->Render() | color(_wasCopied ? Color::Green : Color::Yellow) | vcenter
            }));

            infoLines.push_back(hbox({
                text(" Time: ") | bold | vcenter,
                text(std::to_string(_time.count()) + " ms") | vcenter
            }));

            if (_status == 1) {
                infoLines.push_back(separatorEmpty());
                infoLines.push_back(text(" Warning: Explorer could not be opened automatically.") | color(Color::Yellow) | dim | center);
            }
        }
        else {
            std::string errorMsg = (_status == 2) ? "Error: Could not create folder." : "Error: File access denied.";
            infoLines.push_back(text(errorMsg) | color(Color::RedLight) | center);
        }

        return vbox({
            filler(),
            vbox({
                separatorEmpty(),
                text(isSuccess ? " FILE SAVED SUCCESSFULLY " : " FAILED TO SAVE FILE ") | bold | color(headerColor) | center | size(WIDTH, GREATER_THAN, 35),
                separatorEmpty(),
                separator(),
                vbox(std::move(infoLines)),
                separator(),
                hbox({
                    _nextGenButton->Render() | flex,
                    text(" "),
                    _exitButton->Render() | flex
                }) | vcenter
            }) | borderRounded | center | size(WIDTH, GREATER_THAN, 50),
            filler(),
            ControlsInfo({
                KeyInfo { "Arrows", "Navigate" },
                KeyInfo { "Enter", "Confirm" },
                KeyInfo { "ESC", "Exit" }
            })
        }) | border | flex;
    });
}

Component tui::SaveResultView::GetComponent()
{
    return _renderer;
}

bool tui::SaveResultView::OnEvent(Event event)
{
    if (event == Event::Escape) {
        _onExit();
        return true;
    }
    return false;
}