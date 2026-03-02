#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <ratio>
#include <string>
#include <type_traits>
#include <utility>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "GenerateView.hpp"
#pragma endregion

using namespace ftxui;

tui::GenerateView::GenerateView(Shape*& outShape, GenerateDuration& outTime, std::function<void()> onFinish)
    : _outShape(outShape), _outTime(outTime), _onFinish(std::move(onFinish))
{
    _renderer = Renderer([this] {
        if (_future.valid() && _future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto [shape, time] = _future.get();
            _outShape = shape;
            _outTime = time;
            _onFinish();
        }

        return vbox({
            filler(),
            vbox({
                text("Generating " + _shapeStr + "\n\n") | center | bold,
                spinner(18, (_frameIndex++) / 3) | center
            }) | center,
            filler()
        }) | borderRounded;
    });
}

void tui::GenerateView::StartTask(std::string shapeName, GenerateTask task)
{
    _shapeStr = shapeName;
    _task = std::move(task);

    StartWorker();
}

Component tui::GenerateView::GetComponent()
{
    return _renderer;
}

bool tui::GenerateView::OnEvent(Event event)
{
    return true;
}

void tui::GenerateView::StartWorker() {
    _future = std::async(std::launch::async, [this]() {
        return _task();
    });
}