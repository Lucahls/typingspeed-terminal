#ifndef TYPINGSPEEDTERMINAL_TYPESPEED_H
#define TYPINGSPEEDTERMINAL_TYPESPEED_H

#include <ftxui/component/screen_interactive.hpp>
#include "Frame.h"

namespace tts {

    namespace Frames {
        class Frame;
    }

    class TypingSpeedTerminal {
    private:
        tts::Frames::Frame*      _frame;
        ftxui::ScreenInteractive _screen = ftxui::ScreenInteractive::Fullscreen();
        ftxui::Closure           _end_loop;
        bool                     _is_running = false;
    public:
        TypingSpeedTerminal();
        void                  start();
        std::function<void()> exit();
        void                  change_to(tts::Frames::Frame* screen);
    };

}

#endif //TYPINGSPEEDTERMINAL_TYPESPEED_H

