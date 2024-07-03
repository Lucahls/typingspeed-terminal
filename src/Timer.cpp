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
    }

    int Timer::remaining() const {
        return std::chrono::duration_cast<std::chrono::seconds>(target_time - std::chrono::system_clock::now()).count();
    }

    bool Timer::finished() {
        return std::chrono::system_clock::now() > target_time;
    }
} // tts