//
// Created by Luca Hessels on 22.06.24.
//

#ifndef TYPINGSPEEDTERMINAL_TIMER_H
#define TYPINGSPEEDTERMINAL_TIMER_H

namespace tts {

    class Timer {
    private:
        int _seconds;
    public:
        Timer(int seconds) : _seconds(seconds) {};
        void start();
        int remaining();
        bool done();
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_TIMER_H
