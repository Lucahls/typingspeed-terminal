//
// Created by Luca Hessels on 22.06.24.
//

#ifndef TYPINGSPEEDTERMINAL_TIMER_H
#define TYPINGSPEEDTERMINAL_TIMER_H

#include <functional>
#include <utility>

namespace tts {

    class Timer {
    protected:
        int                                   _seconds = 0;
        std::chrono::system_clock::time_point _target_time;
        bool                                  _is_running = false;
    public:
        /**
         * Passive timer that actively needs to be asked
         * whether it has finished or not.
         * @param seconds Duration in seconds
         */
        explicit Timer(int seconds) : _seconds(seconds) {};
        /**
         * Start the timer, finishes in '_seconds' seconds
         */
        void     start();
        int      remaining() const;
        bool     finished();
        bool     is_running() const {return _is_running;};
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_TIMER_H
