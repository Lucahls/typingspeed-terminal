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
        //std::function<void()> _callback = nullptr;
        //bool finished = false;
    public:
        explicit Timer(int seconds) : _seconds(seconds) {};
        void start();
        int remaining() const;
        bool finished();
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_TIMER_H
