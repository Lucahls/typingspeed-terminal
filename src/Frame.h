#ifndef TYPINGSPEEDTERMINAL_FRAME_H
#define TYPINGSPEEDTERMINAL_FRAME_H

#include <ftxui/component/component.hpp>
#include "TypingSpeedTerminal.h"

namespace tts {
    class TypingSpeedTerminal;
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
    protected:
        std::string _input;
        ftxui::Component _input_field;
        std::string _sentence = "This is the testing sentence to type";
    public:
        explicit TypingTerminal(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
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
