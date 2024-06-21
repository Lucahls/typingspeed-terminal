#include "TypingSpeedTerminal.h"
#include <fmt/core.h>

tts::TypingSpeedTerminal::TypingSpeedTerminal() {
    // this->_screen.
    this->_frame = new tts::Frames::TypingTerminal(this);
}

void tts::TypingSpeedTerminal::start() {
    this->_is_running = true;
    //while(this->_is_running)
        if(this->_frame != nullptr) {
            this->_screen.Loop(this->_frame->render());
        }
        else
            this->_is_running = false;

}

std::function<void()> tts::TypingSpeedTerminal::exit() {
    return [&] {
        this->_is_running = false;
        this->_screen.Exit();
    };
}

void tts::TypingSpeedTerminal::change_to(tts::Frames::Frame* frame) {
    this->_frame = frame;
    this->_screen.Exit();
}
