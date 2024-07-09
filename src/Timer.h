//
// Created by Luca Hessels on 22.06.24.
//

#ifndef TYPINGSPEEDTERMINAL_TIMER_H
#define TYPINGSPEEDTERMINAL_TIMER_H

#include <functional>
#include <utility>

namespace tts {

    class Timer {
    private:
        int _seconds = 0;
        std::chrono::system_clock::time_point target_time;
        bool _is_running = false;
    public:
        explicit Timer(int seconds) : _seconds(seconds) {};
        void start();
        int remaining() const;
        bool finished();
        bool is_running() {return _is_running;};
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_TIMER_H
