#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <chrono>
#include <functional>
#include <string>
#include <ratio>
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
#include "SaveFormatView.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

tui::SaveFormatView::SaveFormatView(std::string& shapeName, std::chrono::duration<double, std::milli>& time, std::function<void(int)> onSave, std::function<void()> onCancel)
    : _shapeName(shapeName), _time(time), _onSave(std::move(onSave)), _onCancel(std::move(onCancel))
{
    RadioboxOption r_options;
    r_options.entries = std::move(&_formatOptions);
    r_options.selected = &_selectedFormat;
    r_options.transform = [this](const EntryState& s) -> Element {
#if defined(_WIN32)
        // Microsoft terminal do not use fonts able to render properly the default
        // radiobox glyph.
        auto prefix = text(s.state ? "(*) " : "( ) ");  // NOLINT
#else
        auto prefix = text(s.state ? "◉ " : "○ ");  // NOLINT
#endif
        auto t = text(s.label);
        if (s.active) {
            t |= bold;
        }
        if (s.focused) {
            t |= inverted;
        }
        Element row = hbox({ prefix, t });

        if (s.index == 0) {
            return vbox({
                text(" STRUCT (Data layout) ") | bold | color(Color::BlueLight),
                separatorLight(),
                row
            });
        }
        else if (s.index == 4) {
            return vbox({
                separatorEmpty(),
                text(" VALUES (Raw Floats) ") | bold | color(Color::BlueLight),
                separatorLight(),
                row
            });
        }
        else if (s.index == 8) {
            return vbox({
                separatorEmpty(),
                text(" EXPORT (External Files) ") | bold | color(Color::BlueLight),
                separatorLight(),
                row
            });
        }

        return row;
    };
    _formatRadio = Radiobox(r_options);

    ButtonOption b_options;
    b_options.transform = [](const EntryState& s) {
        auto element = vbox({ text(s.label) | hcenter }) | borderLight;
        if (s.focused) {
            element |= inverted;
        }
        return element;
    };

    _saveButton = Button("Save", [&] {
        _onSave(_selectedFormat);
    }, b_options);

    _cancelButton = Button("Cancel", [&] {
        _onCancel();
    }, b_options);

    _container = Container::Vertical({
        _formatRadio,
        Container::Horizontal({
            _cancelButton,
            _saveButton
        })
    });

    _renderer = Renderer(_container, [this] {
        return vbox({
            filler(),
            vbox({
                hbox({
                    filler(),
                    text(" GENERATION COMPLETE ") | bold | color(Color::GreenLight),
                    filler()
                }),
                separator(),

                vbox({
                    hbox({ text(" Shape:    ") | bold, text(_shapeName)}),
                    hbox({ text(" Time:     ") | bold, text(std::to_string(_time.count()) + " ms") })
                }) | center | size(WIDTH, GREATER_THAN, 40),

                separator(),

                vbox({
                    text("SELECT FORMAT") | bold | center | color(Color::Magenta2),
                    separator(),
                    _formatRadio->Render() | vscroll_indicator | yframe | size(HEIGHT, LESS_THAN, 12) | size(WIDTH, GREATER_THAN, 40)
                }) | hcenter,

                separator(),

                hbox({
                    _cancelButton->Render() | flex,
                    text(" "),
                    _saveButton->Render() | flex
                })
            }) | borderRounded | center | size(WIDTH, GREATER_THAN, 60),
            filler(),
            ControlsInfo({
                KeyInfo { "Arrows", "Navigate" },
                KeyInfo { "Enter", "Confirm" },
                KeyInfo { "ESC", "Cancel" }
            })
        }) | borderRounded;
    });
}

Component tui::SaveFormatView::GetComponent()
{
    return _renderer;
}

bool tui::SaveFormatView::OnEvent(Event event)
{
    if (event == Event::Escape) {
        _onCancel();
        return true;
    }
    return false;
}