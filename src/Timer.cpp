//
// Created by Luca Hessels on 22.06.24.
//

#include "Timer.h"
#include <chrono>
#include <thread>

namespace tts {
    void Timer::start() {
        std::thread thread([](int* s){
            while(*s > 0) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                *s -= 1;
            }
            }, &(this->_seconds));
        thread.detach();
    }
    int Timer::remaining() {
        return _seconds;
    }
} // tts