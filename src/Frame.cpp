#include <ftxui/component/screen_interactive.hpp>
#include "Frame.h"

tts::Frames::Home::Home(tts::TypingSpeedTerminal* terminal) : tts::Frames::Frame(terminal){
    this->_button = ftxui::Button("Start Game",
                                  [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal));});
}

ftxui::Component tts::Frames::Home::render() {
    return ftxui::Renderer(this->_button, [&] {
        return this->_button->Render();
    });
}

tts::Frames::TypingTerminal::TypingTerminal(tts::TypingSpeedTerminal* terminal) : tts::Frames::Frame(terminal){
    this->_input_field = ftxui::Input(&this->_input);
    this->_input_field |= ftxui::CatchEvent([&](ftxui::Event event) {
        return event.is_character() && this->_input.length() >= this->_sentence.length();
    });
}

ftxui::Component tts::Frames::TypingTerminal::render() {
    return ftxui::Renderer(this->_input_field, [&] {
        return ftxui::flexbox({
                ftxui::window(ftxui::text("Timer: 1min"),
                              ftxui::text(this->_input)),
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::AlignContent::Center)
            .Set(ftxui::FlexboxConfig::JustifyContent::Center));
    });
}

tts::Frames::Stats::Stats(tts::TypingSpeedTerminal* terminal) : tts::Frames::Frame(terminal) {
    this->_button = ftxui::Button("Restart",
                                  [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal));});

}

ftxui::Component tts::Frames::Stats::render() {
    return ftxui::Renderer(this->_button, [&] {
        return this->_button->Render();
    });
}
