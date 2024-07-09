//
// Created by Luca Hessels on 22.06.24.
//

#include "Timer.h"
#include <chrono>
#include <thread>

namespace tts {
    void Timer::start() {
        auto start_time = std::chrono::system_clock::now();
        target_time = start_time + std::chrono::duration(std::chrono::seconds(_seconds));
        _is_running = true;
    }

    int Timer::remaining() const {
        if(_is_running)
            return std::chrono::duration_cast<std::chrono::seconds>(target_time - std::chrono::system_clock::now()).count();
        else
            return _seconds;
    }

    bool Timer::finished() {
        return _is_running && std::chrono::system_clock::now() > target_time;
    }
} // tts