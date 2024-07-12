#include "Frame.h"
#include "Quotes.h"
#include "AsciiArt.h"

#include <ftxui/component/screen_interactive.hpp>

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
        this->_button_start = ftxui::Button("Press [↩] to start", [&] {
            this->_terminal->change_to(std::make_unique<TypingTerminal>(this->_terminal));
            }, ftxui::ButtonOption::Simple()) | ftxui::color(ftxui::Color::LightGreen);

        this->_button_config = ftxui::Button("Settings", [&] {
            this->_terminal->change_to(std::make_unique<Config>(this->_terminal));
        });
    }

    ftxui::Component Home::render() {
        return ftxui::Renderer(ftxui::Container::Vertical({_button_start, _button_config}), [&] {
            return ftxui::flexbox({
                title_ascii_art(),
                ftxui::vbox({
                    _button_start->Render(),
                    _button_config->Render(),
                })
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::Direction::Column)
            .Set(ftxui::FlexboxConfig::JustifyContent::SpaceAround)
            .Set(ftxui::FlexboxConfig::AlignContent::Center)
            .Set(ftxui::FlexboxConfig::AlignItems::Center));
        });
    }

    /**
     * Config
     */
    Config::Config(tts::TypingSpeedTerminal *terminal) : Frame(terminal) {
        this->_button = ftxui::Button(" ← Go Back ",[&] {
                _save_tags();
                this->_terminal->change_to(std::make_unique<Home>(this->_terminal));
            }, ftxui::ButtonOption::Simple());

        // Build the list of tags from which users can choose in config
        _tags = Quotes::tags();
        _tags_checkbox = ftxui::Container::Vertical({});
        _tags_states = std::make_unique<bool[]>(_tags.size());
        for (int i = 0; i < _tags.size(); ++i) {
            _tags_states[i] = false;
            std ::string label = std::get<0>(_tags[i]) + "(" + std::to_string(std::get<1>(_tags[i])) + ")";
            _tags_checkbox->Add(ftxui::Checkbox(label, &_tags_states[i]));
        }
    }

    ftxui::Component Config::render() {
        return ftxui::Renderer(ftxui::Container::Vertical({_tags_checkbox, _button}), [&] {
            return ftxui::flexbox({
                ftxui::text("Choose movies") | ftxui::bold,
                ftxui::text("Only quotes from selected") | ftxui::dim,
                ftxui::text("movies are shown while typing") | ftxui::dim,
                ftxui::text(""),
                ftxui::flexbox({
                    _tags_checkbox->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 10)
                }, ftxui::FlexboxConfig().Set(ftxui::FlexboxConfig::Direction::Row)),
                ftxui::text(""),
                _button->Render()
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::Direction::Column)
            .Set(ftxui::FlexboxConfig::JustifyContent::Center)
            .Set(ftxui::FlexboxConfig::AlignContent::Center));
        });
    }

    void Config::_save_tags() {
        std::vector<std::string> selected_tags;
        for (int i = 0; i < _tags.size(); ++i)
            if (_tags_states[i])
                selected_tags.push_back(std::get<0>(_tags[i]));
        Quotes::filter(selected_tags);
    }

    /**
     * TypingTerminal
     */
    TypingTerminal::TypingTerminal(TypingSpeedTerminal *terminal)
        : Frame(terminal), _timer(_seconds) {
        this->_typing_states = std::vector<tts::TypingState>(_typing_text.length(), tts::TypingState::EMPTY);
        this->_input_field = ftxui::Input(&this->_input);
        this->_quote = Quotes::quote();
        this->_typing_text = std::get<0>(_quote);

        // CatchEvent is called BEFORE _input is updated,
        // therefore does not contain the currently typed key
        this->_input_field |= ftxui::CatchEvent([&](ftxui::Event event) {
            // Start the timer on first Keyboard-Input
            if(!_timer.is_running() && event.is_character())
                _timer.start();

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
            // _is_over prevents the renderer from calling this piece multiple times
            // (even though this shouldn't happen in the first place)
            // and therefore calculating the statistics too often
            if(_timer.finished() && !_is_over) {
                _is_over = true;
                _keep_statistics_chars();
                this->_terminal->change_to(std::make_unique<Stats>(this->_terminal,
                                                                         stats, _seconds));
            }

            int remain = _timer.remaining();
            ftxui::Decorator timer_color = ftxui::color(ftxui::Color::White);
            if(remain < _seconds/10)
                timer_color = ftxui::color(ftxui::Color::RedLight);
            else if(remain < _seconds/4)
                timer_color = ftxui::color(ftxui::Color::YellowLight);

            ftxui::Element colored_text = ftxui::hbox(_generate_colored_text(_input));

            // Last character was inputted for this sentence, switch to next
            if(_input_index == _typing_text.length())
                _next();

            return ftxui::flexbox({
                to_ascii_art(remain) | timer_color,
                ftxui::text("seconds remaining"),
                ftxui::text(""),
                ftxui::text(""),
                ftxui::vbox({
                    colored_text,
                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text("~ " + std::get<1>(_quote)) | ftxui::color(ftxui::Color::GrayLight) | ftxui::dim
                    })
                }),
            }, ftxui::FlexboxConfig()
            .Set(ftxui::FlexboxConfig::AlignContent::Center)
            .Set(ftxui::FlexboxConfig::JustifyContent::Center)
            .Set(ftxui::FlexboxConfig::AlignItems::Center)
            .Set(ftxui::FlexboxConfig::Direction::Column));
        });
    }

    void TypingTerminal::_keep_statistics_keys(const ftxui::Event& input) {
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
        _keep_statistics_chars();
        _quote = Quotes::quote();
        _typing_text = std::get<0>(_quote);
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

        for (int i = 0; i < _typing_states.size(); i++) {
            // Color each character corresponding to its correctness
            ftxui::Element el = ftxui::text(std::string{this->_typing_text[i]});
            if(i != _input_index)
                el |= ftxui::color(tts::TypingColors[_typing_states[i]]);
            else {
                // Highlight the cursor position
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
        this->_button_restart = ftxui::Button(" ⟳  Restart ", [&] {
                this->_terminal->change_to(std::make_unique<TypingTerminal>(this->_terminal));
        });
        this->_button_quit = ftxui::Button(" × Quit ", this->_terminal->exit());

        // Calculation for Net WPM from https://www.speedtypingonline.com/typing-equations
        _wpm = ((_stats.correct / 5.f) - _stats.mistakes) / (seconds / 60.f);
        if(_wpm < 0)
            _wpm = 0;
        _accuracy = (_stats.correct_keystrokes / (float)(_stats.correct_keystrokes + _stats.wrong_keystrokes)) * 100;
    }

    ftxui::Component Stats::render() {
        return ftxui::Renderer(ftxui::Container::Horizontal({this->_button_quit,
                                                                  this->_button_restart}), [&] {

            ftxui::Decorator wpm_color = ftxui::color(ftxui::Color::LightGreen);
            if(_wpm < 30)
                wpm_color = ftxui::color(ftxui::Color::RedLight);
            else if(_wpm < 40)
                wpm_color = ftxui::color(ftxui::Color::YellowLight);

            ftxui::Decorator accuracy_color = ftxui::color(ftxui::Color::LightGreen);
            if(_accuracy < 80)
                accuracy_color = ftxui::color(ftxui::Color::RedLight);
            else if(_accuracy < 90)
                accuracy_color = ftxui::color(ftxui::Color::YellowLight);

            return ftxui::flexbox({
                // ---- Header ----
                to_ascii_art(_wpm) | wpm_color,
                ftxui::text("Words per minute"),
                ftxui::hbox({
                   ftxui::text(std::to_string(_accuracy) + "%") | accuracy_color,
                   ftxui::text(" accuracy")
                }),
                ftxui::text(""),

                // ---- Detailed stats ----
                /*
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
                 */
                ftxui::text(""),

                // ---- Buttons ----
                ftxui::hbox({
                        ftxui::filler(),
                        this->_button_quit->Render(),
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