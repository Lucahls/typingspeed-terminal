#include "TypingSpeedTerminal.h"
#include <fmt/core.h>
#include <ftxui/component/loop.hpp>
#include <utility>

namespace tts {
    TypingSpeedTerminal::TypingSpeedTerminal() {
        this->_next_frame = std::make_unique<Frames::Home>(this);
    }

    void TypingSpeedTerminal::start() {
        _is_running = true;
        // Update the screen continuously through an "empty" Event,
        // otherwise it would only update on every user Event (e.g. keyboard)
        std::thread refresh_ui([&] {
            while (_is_running) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(50ms);
                // Request a new frame to be drawn by simulating a
                // new "custom" event to be handled.
                _screen.Post(ftxui::Event::Custom);
            }
        });
        // Main-Loop
        while (_next_frame != nullptr) {
            _frame = std::move(_next_frame);
            _next_frame = nullptr;
            _screen.Loop(_frame->render());
        }
        _is_running = false;
        refresh_ui.join();
    }

    std::function<void()> TypingSpeedTerminal::exit() {
        return [&] {
            change_to(nullptr);
        };
    }

    void TypingSpeedTerminal::change_to(
            std::unique_ptr<Frames::Frame> frame) {
        _next_frame = std::move(frame);
        _screen.Exit();
    }

}