#ifndef TYPINGSPEEDTERMINAL_FRAME_H
#define TYPINGSPEEDTERMINAL_FRAME_H

#include <ftxui/component/component.hpp>
#include "TypingSpeedTerminal.h"
#include "Timer.h"

namespace tts {
    ftxui::Element int_to_ascii_art(int number);
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
        std::string last = "";
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
        ftxui::Component _button;
    public:
        explicit         Home(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };

    class Stats;

    class TypingTerminal : public Frame {
    public:
        explicit TypingTerminal(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
        TypingStats stats = TypingStats();
        void change_to_stats();
    protected:
        int                           _seconds = 600;
        Timer                         _timer;
        std::string                   _input;
        ftxui::Component              _input_field;
        int                           _input_index = 0;
        std::string                   _typing_text;
        std::vector<tts::TypingState> _typing_states;
        std::vector<tts::TypingState> _check_text(std::string const &text, std::string const &verify);
        std::vector<ftxui::Element>   _generate_colored_text(std::string &text);
        void                          _keep_statistics(const ftxui::Event& input);
        void                          _next();
    };


    class Stats : public Frame {
    protected:
        ftxui::Component _button_restart;
        ftxui::Component _button_quit;
        TypingStats _stats;
    public:
        explicit Stats(tts::TypingSpeedTerminal* terminal, TypingStats stats);
        ftxui::Component render() override;
    };
}

#endif //TYPINGSPEEDTERMINAL_FRAME_H
