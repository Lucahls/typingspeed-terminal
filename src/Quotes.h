//
// Created by Luca Hessels on 21.06.24.
//

#ifndef TYPINGSPEEDTERMINAL_QUOTES_H
#define TYPINGSPEEDTERMINAL_QUOTES_H

#include <string>

namespace tts {

    class Quotes {
    protected:
        static std::string _fetch_quote();
    public:
        static std::vector<std::string> tags();
        static std::string quote();
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_QUOTES_H
