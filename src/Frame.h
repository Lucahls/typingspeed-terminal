#ifndef TYPINGSPEEDTERMINAL_FRAME_H
#define TYPINGSPEEDTERMINAL_FRAME_H

#include <ftxui/component/component.hpp>
#include "TypingSpeedTerminal.h"

namespace tts {
    class TypingSpeedTerminal;

    enum class TypingState {
        CORRECT = 0,
        WRONG,
        EMPTY,
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
        virtual ftxui::Component render() = 0;
    };


    class Home : public Frame {
    protected:
        ftxui::Component _button;
    public:
        explicit Home(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };


    class TypingTerminal : public Frame {
    public:
        explicit TypingTerminal(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    protected:
        std::string _input;
        ftxui::Component _input_field;
        std::string _typing_text = "This is the testing sentence to type";
        std::vector<tts::TypingState> _check_text(std::string const &text, std::string const &verify) const;
        std::vector<ftxui::Element> _generate_colored_text(std::string &text) const;
    };


    class Stats : public Frame {
    protected:
        ftxui::Component _button;
    public:
        explicit Stats(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };
}

#endif //TYPINGSPEEDTERMINAL_FRAME_H
