#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <string>
#include <ratio>
#include <type_traits>
#include <utility>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "SaveView.hpp"
#pragma endregion

using namespace ftxui;

tui::SaveView::SaveView(Shape*& shapeToSave, SaveDuration& outTime, std::string& saveLocation, std::function<void()> onFinish)
    : _shapeToSave(shapeToSave), _outTime(outTime), _saveLocation(saveLocation), _onFinish(std::move(onFinish))
{
    _renderer = Renderer([this] {
        if (_future.valid() && _future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto [loc, t] = _future.get();
            _saveLocation = loc;
            _outTime = t;
            _onFinish();
        }

        return vbox({
            filler(),
            vbox({
                text("Saving to " + _fileTypeStr + " file\n\n") | center | bold,
                spinner(18, (_frameIndex++) / 3) | center
            }) | center,
            filler()
        }) | borderRounded;
    });
}

void tui::SaveView::StartTask(std::string fileType, SaveTask task)
{
    _fileTypeStr = fileType;
    _task = std::move(task);

    StartWorker();
}

Component tui::SaveView::GetComponent()
{
    return _renderer;
}

bool tui::SaveView::OnEvent(Event event)
{
    return true;
}

void tui::SaveView::StartWorker()
{
    _future = std::async(std::launch::async, [this]() {
        return _task();
    });
}