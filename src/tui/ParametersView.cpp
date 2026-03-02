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
#include <ftxui/screen/color.hpp>
#pragma endregion

#pragma region MY_FILES
#include "ParametersView.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

tui::ParametersView::ParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : _rangeIndex(rangeIndex), _onGenerate(std::move(onGenerate)), _onBack(std::move(onBack))
{
    _generateButtonBase = Button("Generate", [&] {
        if (AreAllInputsValid()) _onGenerate();
    });

    _generateButton = Renderer(_generateButtonBase, [this] {
        const bool allValid = AreAllInputsValid();

        auto btn = _generateButtonBase->Render();

        if (!allValid) {
            btn = btn | color(Color::GrayDark);
            btn = btn | dim;
        }

        return btn;
    });

    _backButton = Button("Back", [&] {
        _onBack();
    });

    _rangeRadio = Radiobox(&_rangeOptions, &rangeIndex);
}

Component tui::ParametersView::GetComponent()
{
    return _renderer;
}

bool tui::ParametersView::OnEvent(Event event)
{
    if (event == Event::Escape) {
        _onBack();
        return true;
    }
    return false;
}

Element tui::ParametersView::RenderBase()
{
    Elements content;
    content.push_back(
        hbox({
            _backButton->Render(),
            filler(),
            text(_header) | bold | color(Color::Cyan1) | center,
            filler()
        }) | size(WIDTH, GREATER_THAN, 50)
    );
    content.push_back(separator());
    content.push_back(AsciiArt());
    content.push_back(separator());

    if (_dynamicContentNotEmpty) {
        content.push_back(_dynamicContent->Render() | vscroll_indicator | yframe | size(HEIGHT, LESS_THAN, 10));
        content.push_back(separator());
    }

    content.push_back(_generateButton->Render() | center);

    return vbox({
        vbox(content) | borderRounded | center,
        filler(),
        ControlsInfo({
            KeyInfo { "Arrows", "Navigate" },
            KeyInfo { "Enter", "Confirm" },
            KeyInfo { "ESC", "Back" }
        })
    }) | border | flex;
}

bool tui::ParametersView::AreAllInputsValid() const
{
    return true;
}

Element tui::ParametersView::AsciiArt() const
{
    return vbox({
        text("  +------+"),
        text(" /      /|"),
        text("+------+ |"),
        text("|      | +"),
        text("|      |/ "),
        text("+------+  "),
        }) | hcenter;
}