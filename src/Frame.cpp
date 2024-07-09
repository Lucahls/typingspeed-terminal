#include "Frame.h"
#include "Quotes.h"
#include "AsciiArt.h"

#include <ftxui/component/screen_interactive.hpp>
#include <utility>

namespace tts {
    std::unordered_map<TypingState, ftxui::Color> TypingColors = {
            {TypingState::CORRECT, ftxui::Color::LightGreen},
            {TypingState::WRONG, ftxui::Color::Red},
            {TypingState::EMPTY, ftxui::Color::GrayLight},
    };
}

namespace tts::Frames {
    /**
     * Home
     */
    Home::Home(TypingSpeedTerminal *terminal) : Frame(terminal) {
        this->_button = ftxui::Button("Press [↩] to start",
                                      [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal)); },
                                      ftxui::ButtonOption::Simple()) | ftxui::color(ftxui::Color::LightGreen);
    }

    ftxui::Component Home::render() {
        return ftxui::Renderer(this->_button, [&] {
            return ftxui::flexbox({
                title_ascii_art(),
                ftxui::flexbox({
                    this->_button->Render(),
                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::JustifyContent::Center))
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::Direction::Column)
            .Set(ftxui::FlexboxConfig::JustifyContent::SpaceAround)
            .Set(ftxui::FlexboxConfig::AlignContent::Center));
        });
    }

    /**
     * TypingTerminal
     */
    TypingTerminal::TypingTerminal(TypingSpeedTerminal *terminal)
        : Frame(terminal), _timer(_seconds) {
        this->_typing_states = std::vector<tts::TypingState>(_typing_text.length(), tts::TypingState::EMPTY);
        this->_input_field = ftxui::Input(&this->_input);
        this->_typing_text = Quotes::quote();
        //_timer.start();
        // CatchEvent is called BEFORE _input is updated,
        // therefore does not contain the currently typed key
        this->_input_field |= ftxui::CatchEvent([&](ftxui::Event event) {
            // Start the timer on first Keyboard-Input
            if(!_timer.is_running() && event.is_character())
                _timer.start();

            if(event == ftxui::Event::Return) {
                _keep_statistics_chars();
                _next();
                return true;
            }
            // Manually keep track of the cursor position in the input,
            // as there seems to be no way to get it from the input component directly
            if(event.is_character() and _input.length() < _typing_text.length()) {
                _input_index += _input_index <= _input.length() ? 1 : 0;
                _keep_statistics_keys(event);
            }
            else if(event == ftxui::Event::ArrowRight)
                _input_index += _input_index < _input.length() ? 1 : 0;
            else if(event == ftxui::Event::Backspace or event == ftxui::Event::ArrowLeft)
                _input_index -= _input_index > 0 ? 1 : 0;
            else
                return true;

            return false;
        });
    }

    ftxui::Component TypingTerminal::render() {
        return ftxui::Renderer(this->_input_field, [&] {
            if(_timer.finished()) {
                _keep_statistics_chars();
                this->_terminal->change_to(new Stats(this->_terminal, stats, _seconds));
            }

            int remain = _timer.remaining();
            return ftxui::flexbox({
                to_ascii_art(remain) | (remain > _seconds/4 ? ftxui::color(ftxui::Color::White) : remain > _seconds/10 ? ftxui::color(ftxui::Color::Orange1) : ftxui::color(ftxui::Color::Red1)),
                ftxui::text("seconds remaining"),
                ftxui::text(""),
                ftxui::hbox(_generate_colored_text(_input)),
                //ftxui::vbox(
                //    ftxui::text(fmt::format("Keystrokes: {}; Correct: {}; Wrong: {}",
                //                            stats.correct_keystrokes + stats.wrong_keystrokes,
                //                            stats.correct_keystrokes, stats.wrong_keystrokes)),
                //    ftxui::text(fmt::format("Input: {}; Input length: {}; Index: {}",
                //                            _input, _input.length(), _input_index))
                //    )

                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::AlignContent::Center)
                .Set(ftxui::FlexboxConfig::JustifyContent::Center)
                .Set(ftxui::FlexboxConfig::AlignItems::Center)
                .Set(ftxui::FlexboxConfig::Direction::Column));
        });
    }

    void TypingTerminal::_keep_statistics_keys(const ftxui::Event& input) {
        // TODO: Somehow count corrected mistakes, maybe using a 'last-state'
        //  value/array for each character, when the last state was wrong,
        //  but now it is correct => corrected_mistakes++

        // Undo a typing mistake
        if(ftxui::Event::Backspace == input && _typing_states[_input_index] == TypingState::WRONG)
            stats.mistakes--;

        // Only count non-function keys (ignore backspace, shift, ...)
        if(!input.is_character())
            return;

        std::string last_char = std::string(1, _typing_text[_input_index-1]);
        if(last_char == input.character())
            stats.correct_keystrokes++;
        else
            stats.wrong_keystrokes++;
    }

    void TypingTerminal::_keep_statistics_chars() {
        for(TypingState state : _typing_states){
            switch (state) {
                case TypingState::CORRECT:
                    stats.correct++;
                    break;
                case TypingState::WRONG:
                    stats.mistakes++;
                    break;
                default:
                    break;
            }
        }
    }

    void TypingTerminal::_next() {
        _typing_text = Quotes::quote();
        _input = "";
        _input_index = 0;
    }

    std::vector<tts::TypingState> TypingTerminal::_check_text(const std::string &text,
                                                              const std::string &verify) {
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

    std::vector<ftxui::Element> TypingTerminal::_generate_colored_text(std::string &text) {
        _typing_states = _check_text(this->_typing_text, text);
        std::vector<ftxui::Element> elements = std::vector<ftxui::Element>(_typing_states.size());
        // int cursor = (int)text.length();

        for (int i = 0; i < _typing_states.size(); i++) {
            // Color each character responding to its correctness
            ftxui::Element el = ftxui::text(std::string{this->_typing_text[i]});
            if(i != _input_index)
                el |= ftxui::color(tts::TypingColors[_typing_states[i]]);
            else {
                el |= ftxui::color(ftxui::Color::Black);
                el |= ftxui::bgcolor(ftxui::Color::White);
            }
            if (_typing_states[i] == tts::TypingState::WRONG)
                el |= ftxui::underlined;

            elements[i] = el;
        }
        return elements;
    }


    /*
     * Stats
     */
    Stats::Stats(TypingSpeedTerminal *terminal, TypingStats& stats, int seconds)
        : Frame(terminal), _stats(stats) {
        this->_button_restart = ftxui::Button("Restart",
                                      [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal)); });
        this->_button_quit = ftxui::Button("Quit", this->_terminal->exit());
        // Calculation for Net WPM from https://www.speedtypingonline.com/typing-equations
        _wpm = ((_stats.correct / 5.f) - _stats.mistakes) / (seconds / 60.f);
    }

    ftxui::Component Stats::render() {
        return ftxui::Renderer(ftxui::Container::Horizontal({this->_button_quit,
                                                             this->_button_restart}) , [&] {
            return ftxui::flexbox({
                // Header
               to_ascii_art(_wpm) | ftxui::bold | ftxui::color(ftxui::Color::LightGreen),
               ftxui::text("Words per minute"),
               ftxui::text(""),

               // Detailed stats
                ftxui::flexbox({
                    ftxui::vbox(
                        ftxui::text("Keystrokes") | ftxui::bold,
                        ftxui::flexbox({
                            ftxui::vbox(
                                ftxui::text("Total"),
                                ftxui::text("Correct"),
                                ftxui::text("Wrong")
                            ) | ftxui::dim,
                            ftxui::vbox(
                                ftxui::text(std::to_string(_stats.correct_keystrokes + _stats.wrong_keystrokes)),
                                ftxui::text(std::to_string(_stats.correct_keystrokes)),
                                ftxui::text(std::to_string(_stats.wrong_keystrokes))
                            )
                        }, ftxui::FlexboxConfig()
                        .Set(ftxui::FlexboxConfig::Direction::Row)
                        .Set(ftxui::FlexboxConfig::JustifyContent::SpaceBetween))
                    ),
                    ftxui::vbox(
                        ftxui::text("Characters") | ftxui::bold,
                        ftxui::flexbox({
                            ftxui::vbox(
                                ftxui::text("Total"),
                                ftxui::text("Correct"),
                                ftxui::text("Wrong")
                            ) | ftxui::dim,
                            ftxui::vbox(
                                ftxui::text(std::to_string(_stats.correct + _stats.mistakes)),
                                ftxui::text(std::to_string(_stats.correct)),
                                ftxui::text(std::to_string(_stats.mistakes))
                            )
                        }, ftxui::FlexboxConfig()
                        .Set(ftxui::FlexboxConfig::Direction::Row)
                        .Set(ftxui::FlexboxConfig::JustifyContent::Stretch))
                    ),
                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::Direction::Row)
                .Set(ftxui::FlexboxConfig::JustifyContent::SpaceAround))
                | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 50),
                ftxui::text(""),

                // Buttons
                ftxui::hbox({
                        this->_button_quit->Render(),
                        ftxui::text("     "),
                        this->_button_restart->Render(),
                })
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::Direction::Column)
            .Set(ftxui::FlexboxConfig::JustifyContent::Center)
            .Set(ftxui::FlexboxConfig::AlignContent::Center)
            .Set(ftxui::FlexboxConfig::AlignItems::Center));
        });
    }
}