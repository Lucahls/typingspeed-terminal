#include <ftxui/component/screen_interactive.hpp>
#include "Frame.h"

namespace tts {
    std::unordered_map<TypingState, ftxui::Color> TypingColors = {
            {TypingState::CORRECT, ftxui::Color::GrayLight},
            {TypingState::WRONG, ftxui::Color::Red},
            {TypingState::EMPTY, ftxui::Color::GrayDark},
    };
}

namespace tts::Frames {
    Home::Home(TypingSpeedTerminal *terminal) : Frame(terminal) {
        this->_button = ftxui::Button("Start Game",
                                      [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal)); });
    }

    ftxui::Component Home::render() {
        return ftxui::Renderer(this->_button, [&] {
            return this->_button->Render();
        });
    }

    TypingTerminal::TypingTerminal(TypingSpeedTerminal *terminal) : Frame(terminal) {
        this->_input_field = ftxui::Input(&this->_input);
        this->_input_field |= ftxui::CatchEvent([&](ftxui::Event event) {
            return event.is_character() && this->_input.length() >= this->_typing_text.length();
        });
    }

    ftxui::Component TypingTerminal::render() {
        return ftxui::Renderer(this->_input_field, [&] {
            return ftxui::flexbox({
                ftxui::window(ftxui::text("Timer: 1min"),
                              ftxui::hbox(_generate_colored_text(_input))),
                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::AlignContent::Center)
                .Set(ftxui::FlexboxConfig::JustifyContent::Center));
        });
    }


    Stats::Stats(TypingSpeedTerminal *terminal) : Frame(terminal) {
        this->_button = ftxui::Button("Restart",
                                      [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal)); });

    }

    ftxui::Component Stats::render() {
        return ftxui::Renderer(this->_button, [&] {
            return this->_button->Render();
        });
    }


    std::vector<tts::TypingState> TypingTerminal::_check_text(const std::string &text,
                                                              const std::string &verify) const {
        std::vector<tts::TypingState> result = std::vector<tts::TypingState>(text.length(),
                                                                   tts::TypingState::EMPTY);
        for (int i = 0; i < verify.length(); i++) {
            if (text[i] == verify[i])
                result[i] = tts::TypingState::CORRECT;
            else
                result[i] = tts::TypingState::WRONG;
        }
        return result;
    }

    std::vector<ftxui::Element> TypingTerminal::_generate_colored_text(std::string &text) const {
        std::vector<tts::TypingState> char_states = _check_text(this->_typing_text, text);
        std::vector<ftxui::Element> elements = std::vector<ftxui::Element>(char_states.size());
        int cursor = (int)text.length();

        for (int i = 0; i < char_states.size(); i++) {
            // Color each character responding to its correctness
            ftxui::Element el = ftxui::text(std::string{this->_typing_text[i]});
            el |= ftxui::color(tts::TypingColors[char_states[i]]);
            if (char_states[i] == tts::TypingState::WRONG)
                el |= ftxui::underlined;

            if (i == cursor)
                el |= ftxui::bgcolor(ftxui::Color::White);
            elements[i] = el;
        }
        return elements;
    }
}