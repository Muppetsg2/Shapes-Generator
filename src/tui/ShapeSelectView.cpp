#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <ftxui/screen/color.hpp>
#pragma endregion

#pragma region MY_FILES
#include "ShapeSelectView.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

tui::ShapeSelectView::ShapeSelectView(const std::vector<std::string>& shapes, int& selectedShapeIndex, std::function<void()> onConfirm, std::function<void()> onExit, std::function<void()> onEditConfig) 
    : _shapes(shapes), _selectedShapeIndex(selectedShapeIndex), _onConfirm(std::move(onConfirm)), _onExit(onExit), _onEditConfig(std::move(onEditConfig))
{
    auto menu_option = MenuOption::Vertical();
    menu_option.on_enter = [&] { _onConfirm(); };

    _menu = Menu(&_shapes, &_selectedShapeIndex, menu_option);
    _menu = Renderer(_menu, [this] { return _menu->Render() | reflect(_menuBox); }); // Get Menu Box

    _menu |= CatchEvent([this](Event event) {
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed)
        {
            _mouseBtnPressed = true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Released && _mouseBtnPressed) {
            _mouseBtnPressed = false;

            if (_menuBox.Contain(event.mouse().x, event.mouse().y)) {
                _onConfirm();
                return true;
            }
        }
        return false;
    });

    _renderer = Renderer(_menu, [this] {
        _menu->Render() | reflect(_menuBox); // Update Menu Box

        auto title = text("SELECT SHAPE") | bold | color(Color::Magenta2) | hcenter;
        auto subtitle = text("Choose a shape to generate") | dim | hcenter;

        auto menuBox = _menu->Render()
            | vscroll_indicator
            | frame
            | size(WIDTH, EQUAL, 32)
            | size(HEIGHT, LESS_THAN, 12);

        return vbox({
            RenderBanner(),
            filler(),
            vbox({
                title,
                subtitle,
                separator(),
                menuBox
            }) | borderRounded | center,
            filler(),
            ControlsInfo({
                KeyInfo { "Arrows", "Navigate" },
                KeyInfo { "Enter", "Confirm" },
                KeyInfo { "C", "Edit Config" },
                KeyInfo { "ESC", "Exit" }
            })
        }) | border | flex;
    });
}

Component tui::ShapeSelectView::GetComponent()
{
    return _renderer;
}

bool tui::ShapeSelectView::OnEvent(Event event)
{
    if (event == Event::Character('c') || event == Event::Character('C')) {
        _onEditConfig();
        return true;
    }
    else if (event == Event::Escape)
    {
        _onExit();
        return true;
    }

    return false;
}

Element tui::ShapeSelectView::RenderBanner()
{
    const std::string versionTxt = std::string("Version: ") + SHAPES_GENERATOR_VERSION;
    const std::string escTxt = "Press ESC to exit the program";

    std::vector<std::string> bannerLines = {
        "   ____  _                              ____                           _               ",
        "  / ___|| |__   __ _ _ __   ___  ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __   ",
        "  \\___ \\| '_ \\ / _` | '_ \\ / _ \\/ __| | |  _ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|  ",
        "   ___) | | | | (_| | |_) |  __/\\__ \\ | |_| |  __/ | | |  __/ | | (_| | || (_) | |     ",
        "  |____/|_| |_|\\__,_| .__/ \\___||___/  \\____|\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|     ",
        "                    |_|                                                                "
    };

    std::vector<Element> bannerElements;
    for (const auto& line : bannerLines) {
        bannerElements.push_back(text(line) | color(Color::Cyan1));
    }

    auto centerVersion = hbox({ text("Version: ") | dim, text(SHAPES_GENERATOR_VERSION) | bold | color(Color::YellowLight) }) | hcenter;
    auto centerEsc = hbox({ text("Press ") | dim, StyleKey("ESC"), text(" to exit the program") | dim }) | hcenter;

    return vbox({
        vbox(bannerElements),
        centerVersion,
        centerEsc
    }) | border | hcenter;
}