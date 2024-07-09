#ifndef TYPINGSPEEDTERMINAL_FRAME_H
#define TYPINGSPEEDTERMINAL_FRAME_H

#include <ftxui/component/component.hpp>
#include "TypingSpeedTerminal.h"
#include "Timer.h"

namespace tts {
    class TypingSpeedTerminal;

    enum class TypingState {
        CORRECT = 0,
        WRONG,
        EMPTY,
    };

    struct TypingStats {
        // Single Keystroke
        int correct_keystrokes = 0;
        int wrong_keystrokes = 0;
        // Characters in sentence
        int correct = 0;
        int mistakes = 0;
        int corrected_mistakes = 0;
    };

    struct TypingConfig {
        int seconds = 0;
        bool online = false;
    };
}

namespace tts::Frames {

    /**
     * Frame Baseclass to render on screen
     */
    class Frame {
    protected:
        tts::TypingSpeedTerminal* _terminal;
    public:
        explicit Frame(tts::TypingSpeedTerminal* terminal) : _terminal(terminal) {};
        virtual  ftxui::Component render() = 0;
    };


    class Home : public Frame {
    protected:
        ftxui::Component _button_start;
        ftxui::Component _button_config;
    public:
        explicit         Home(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };


    class Config : public Frame {
    private:
        ftxui::Component _button;
        std::vector<ftxui::Component> _tags;
        std::unique_ptr<bool[]> _tags_states;
    public:
        explicit         Config(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };


    class TypingTerminal : public Frame {
    public:
        explicit TypingTerminal(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
        TypingStats stats = TypingStats();
    protected:
        int                           _seconds = 10;
        Timer                         _timer;
        std::string                   _input;
        ftxui::Component              _input_field;
        int                           _input_index = 0;
        std::string                   _typing_text;
        std::vector<tts::TypingState> _typing_states;
        std::vector<tts::TypingState> _check_text(std::string const &text, std::string const &verify);
        std::vector<ftxui::Element>   _generate_colored_text(std::string &text);
        void                          _keep_statistics_keys(const ftxui::Event& input);
        void                          _keep_statistics_chars();
        void                          _next();
    };


    class Stats : public Frame {
    protected:
        ftxui::Component _button_restart;
        ftxui::Component _button_quit;
        TypingStats      _stats;
        int              _wpm;
    public:
        explicit Stats(tts::TypingSpeedTerminal* terminal, TypingStats& stats, int seconds);
        ftxui::Component render() override;
    };
}

#endif //TYPINGSPEEDTERMINAL_FRAME_H
