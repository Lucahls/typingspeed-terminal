#ifndef TYPINGSPEEDTERMINAL_ASCIIART_H
#define TYPINGSPEEDTERMINAL_ASCIIART_H

#include <ftxui/component/component.hpp>

namespace tts {
    /**
     * Create an ASCII-Art from any integer
     *
     * @param number
     * @return ftxui::Element containing the ASCII-Art
     */
    ftxui::Element to_ascii_art(int number);
    /**
     * Create an ASCII-Art from any digit (0-9)
     *
     * @param digit Digit from 0 to 9
     * @return ftxui::Element containing the ASCII-Art
     */
    ftxui::Element to_ascii_art(char digit);
    /**
     * Create the "Typingspeed"-Header
     * @return
     */
    ftxui::Element title_ascii_art();
}

#endif //TYPINGSPEEDTERMINAL_ASCIIART_H
