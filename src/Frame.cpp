#include <ftxui/component/screen_interactive.hpp>
#include <fmt/core.h>
#include "Frame.h"
#include "Quotes.h"
#include <utility>

namespace tts {
    std::unordered_map<TypingState, ftxui::Color> TypingColors = {
            {TypingState::CORRECT, ftxui::Color::LightGreen},
            {TypingState::WRONG, ftxui::Color::Red},
            {TypingState::EMPTY, ftxui::Color::GrayLight},
    };

    ftxui::Element int_to_ascii_art(int number){
        switch (number) {
            case 1:
                return ftxui::vbox({
                    ftxui::text("  _   \n"),
                    ftxui::text(" / |  \n"),
                    ftxui::text(" | |  \n"),
                    ftxui::text(" |_|  \n"),
                    });
            case 2:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("|_  ) \n"),
                    ftxui::text(" / /  \n"),
                    ftxui::text("/___| \n"),
                    });
            case 3:
                return ftxui::vbox({
                    ftxui::text(" ____ \n"),
                    ftxui::text("|__ / \n"),
                    ftxui::text(" |_ \\ \n"),
                    ftxui::text("|___/ \n"),
                    });
            case 4:
                return ftxui::vbox({
                    ftxui::text(" _ _  \n"),
                    ftxui::text("| | | \n"),
                    ftxui::text("|_  _|\n"),
                    ftxui::text("  |_| \n"),
                    });
            case 5:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("| __| \n"),
                    ftxui::text("|__ \\ \n"),
                    ftxui::text("|___/ \n"),
                    });
            case 6:
                return ftxui::vbox({
                    ftxui::text("  __ \n"),
                    ftxui::text(" / / \n"),
                    ftxui::text("/ _ \\\n"),
                    ftxui::text("\\___/\n"),
                    });
            case 7:
                return ftxui::vbox({
                    ftxui::text(" ____ \n"),
                    ftxui::text("|__  |\n"),
                    ftxui::text("  / / \n"),
                    ftxui::text(" /_/  \n"),
                    });
            case 8:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("( _ ) \n"),
                    ftxui::text("/ _ \\ \n"),
                    ftxui::text("\\___/ \n"),
                    });
            case 9:
                return ftxui::vbox({
                    ftxui::text(" ___ \n"),
                    ftxui::text("/ _ \\\n"),
                    ftxui::text("\\_, /\n"),
                    ftxui::text(" /_/ \n"),
                    });
            case 0:
                return ftxui::vbox({
                    ftxui::text("  __\n"),
                    ftxui::text(" /  \\ \n"),
                    ftxui::text("| () |\n"),
                    ftxui::text(" \\__/ \n"),
                    });
            default:
                return ftxui::text("");
        }
    }
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
                ftxui::vbox(
                    ftxui::text("  ______            _                                      __\n"),
                    ftxui::text(" /_  __/_  ______  (_)___  ____ __________  ___  ___  ____/ /\n"),
                    ftxui::text("  / / / / / / __ \\/ / __ \\/ __ `/ ___/ __ \\/ _ \\/ _ \\/ __  / \n"),
                    ftxui::text(" / / / /_/ / /_/ / / / / / /_/ (__  ) /_/ /  __/  __/ /_/ /  \n"),
                    ftxui::text("/_/  \\__, / .___/_/_/ /_/\\__, /____/ .___/\\___/\\___/\\__,_/   \n"),
                    ftxui::text("    /____/_/            /____/    /_/                        "),
                    ftxui::hbox(ftxui::text("presented by "),
                                ftxui::text("Luca") | ftxui::hyperlink("https://github.com/Lucahls")
                    ) | ftxui::dim | ftxui::color(ftxui::Color::White)
                ) | ftxui::color(ftxui::Color::LightGreen),
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
        : Frame(terminal), _timer(10) {
        this->_typing_states = std::vector<tts::TypingState>(_typing_text.length(), tts::TypingState::EMPTY);
        this->_input_field = ftxui::Input(&this->_input);
        this->_typing_text = Quotes::quote();
        _timer.start();
        // CatchEvent is called BEFORE _input is updated,
        // therefore does not contain the currently typed key
        this->_input_field |= ftxui::CatchEvent([&](ftxui::Event event) {
            if(event == ftxui::Event::Return) {
                _next();
                return true;
            }
            // Manually keep track of the cursor position in the input,
            // as there seems to be no way to get it from the input component directly
            if(event.is_character() and _input.length() < _typing_text.length()) {
                _input_index += _input_index <= _input.length() ? 1 : 0;
                _keep_statistics(event);
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
            if(_timer.finished())
                change_to_stats();

            int remain = _timer.remaining();
            std::string remain_str = std::to_string(remain);
            return ftxui::flexbox({
                ftxui::flexbox({
                    ftxui::hbox({int_to_ascii_art(remain_str[0] - '0'),
                                 int_to_ascii_art(remain_str[1] - '0')})
                            | ftxui::color((remain < 15 ? ftxui::Color::Orange1 : ftxui::Color::LightGreen))
                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::JustifyContent::Center)),
                //ftxui::hbox({ftxui::text(fmt::format("{}s",_timer.remaining())) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5),
                //            ftxui::gauge(_timer.remaining()/60.f)  | ftxui::xflex}),
                ftxui::hbox(_generate_colored_text(_input)),
                ftxui::vbox(
                    ftxui::text(fmt::format("Keystrokes: {}; Correct: {}; Wrong: {}",
                                            stats.correct_keystrokes + stats.wrong_keystrokes,
                                            stats.correct_keystrokes, stats.wrong_keystrokes)),
                    ftxui::text(fmt::format("Input: {}; Input length: {}; Index: {}",
                                            _input, _input.length(), _input_index))
                    )

                }, ftxui::FlexboxConfig()
                .Set(ftxui::FlexboxConfig::AlignContent::Center)
                .Set(ftxui::FlexboxConfig::JustifyContent::Center)
                .Set(ftxui::FlexboxConfig::Direction::Column));
        });
    }

    void TypingTerminal::change_to_stats() {
        this->_terminal->change_to(new Stats(this->_terminal, stats));
    }

    void TypingTerminal::_keep_statistics(const ftxui::Event& input) {
        // TODO: Somehow count corrected mistakes, maybe using a 'last-state'
        //  value/array for each character, when the last state was wrong,
        //  but now it is correct => corrected_mistakes++

        // Undo a typing mistake
        if(ftxui::Event::Backspace == input && _typing_states[_input_index] == TypingState::WRONG)
            stats.mistakes--;

        // Only count non-function keys (backspace, shift, ...)
        if(!input.is_character())
            return;

        stats.last = std::string(1, _typing_text[_input_index-1]);
        if(stats.last == input.character())
            stats.correct_keystrokes++;
        else
            stats.wrong_keystrokes++;
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
    Stats::Stats(TypingSpeedTerminal *terminal, TypingStats stats)
        : Frame(terminal), _stats(std::move(stats)) {
        this->_button = ftxui::Button("Restart",
                                      [&] { this->_terminal->change_to(new Frames::TypingTerminal(this->_terminal)); });

    }

    ftxui::Component Stats::render() {
        return ftxui::Renderer(this->_button, [&] {
            return ftxui::flexbox({
                this->_button->Render(),
                ftxui::vbox({
                    ftxui::text("Keystrokes") | ftxui::underlined,
                    ftxui::text(fmt::format("Keystrokes: {}",_stats.correct_keystrokes)),
                    ftxui::text(fmt::format("Correct: {}",_stats.correct_keystrokes)),
                    ftxui::text(fmt::format("Wrong: {}",_stats.wrong_keystrokes)),
                    ftxui::text("Characters") | ftxui::underlined,
                    ftxui::text(fmt::format("Characters: {}",_stats.correct + _stats.mistakes)),
                    ftxui::text(fmt::format("Correct: {}",_stats.correct)),
                    ftxui::text(fmt::format("Wrong: {}",_stats.mistakes)),
                }),
            });
        });
    }
}