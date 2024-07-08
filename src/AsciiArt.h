#ifndef TYPINGSPEEDTERMINAL_ASCIIART_H
#define TYPINGSPEEDTERMINAL_ASCIIART_H

#include <ftxui/component/component.hpp>

namespace tts {
    ftxui::Element to_ascii_art(int number);
    ftxui::Element to_ascii_art(char digit);
    ftxui::Element title_ascii_art();
}

#endif //TYPINGSPEEDTERMINAL_ASCIIART_H
