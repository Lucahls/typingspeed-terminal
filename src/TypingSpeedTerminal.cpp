#include "TypingSpeedTerminal.h"
#include <fmt/core.h>
#include <ftxui/component/loop.hpp>

tts::TypingSpeedTerminal::TypingSpeedTerminal() {
    this->_frame = new tts::Frames::Home(this);
}

void tts::TypingSpeedTerminal::start() {
    _is_running = true;
    //while(this->_is_running)
        if(_frame != nullptr) {
            // Update the UI continuously through an "empty" Event,
            // otherwise it would only update on every user Event (e.g. keyboard)
            std::thread refresh_ui([&] {
                while (_is_running) {
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(0.05s);
                    // Request a new frame to be drawn by simulating a
                    // new "custom" event to be handled.
                    _screen.Post(ftxui::Event::Custom);
                }
            });
            _screen.Loop(_frame->render());
            _is_running = false;
            refresh_ui.join();
        }
        else
            _is_running = false;

}

std::function<void()> tts::TypingSpeedTerminal::exit() {
    return [&] {
        _is_running = false;
        _screen.Exit();
    };
}

void tts::TypingSpeedTerminal::change_to(tts::Frames::Frame* frame) {
    _frame = frame;
    _screen.Loop(_frame->render());
    //this->_screen.Exit();
}
