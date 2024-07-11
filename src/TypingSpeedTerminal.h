#ifndef TYPINGSPEEDTERMINAL_TYPESPEED_H
#define TYPINGSPEEDTERMINAL_TYPESPEED_H

#include <ftxui/component/screen_interactive.hpp>
#include "Frame.h"

namespace tts {

    namespace Frames {
        class Frame;
    }

    /**
     * Typing-Speed-Terminal
     */
    class TypingSpeedTerminal {
    private:
        std::unique_ptr<tts::Frames::Frame>      _frame;
        std::unique_ptr<tts::Frames::Frame>      _next_frame;
        ftxui::ScreenInteractive _screen = ftxui::ScreenInteractive::Fullscreen();
        ftxui::Closure           _end_loop;
        bool                     _is_running = false;
    public:
        TypingSpeedTerminal();
        /* Start the game */
        void start();
        /**
         * Return a function that can exit the game.
         * @return Exit closure
         */
        std::function<void()> exit();
        /**
         * Change the currently rendered screen
         * @param screen Next screen to be rendered
         */
        void change_to(std::unique_ptr<tts::Frames::Frame> screen);
    };

}

#endif //TYPINGSPEEDTERMINAL_TYPESPEED_H

