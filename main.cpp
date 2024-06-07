#include <fmt/core.h>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/component/component.hpp>
#include "src/TypingSpeedTerminal.h"

std::string type_text = "Lol was geht du Kek!";

enum class TypingState {
    CORRECT = 0,
    WRONG = 1,
    EMPTY = 2,
};

std::vector<ftxui::Color> colors = {
        ftxui::Color::GrayLight,
        ftxui::Color::Red,
        ftxui::Color::GrayDark,
};

std::vector<TypingState> typing_check(std::string const &text, std::string const &verify){
    std::vector<TypingState> result = std::vector<TypingState>(text.length(), TypingState::EMPTY);
    for(int i = 0; i < verify.length(); i++) {
        if (text[i] == verify[i])
            result[i] = TypingState::CORRECT;
        else
            result[i] = TypingState::WRONG;
    }
    return result;
}

std::vector<ftxui::Element> text_colored(std::string &text) {
    std::vector<TypingState> text_colors = typing_check(type_text, text);
    std::vector<ftxui::Element> elements = std::vector<ftxui::Element>(text_colors.size());
    int cursor = text.length();
    for(int i = 0; i < text_colors.size(); i++) {
        ftxui::Element el = ftxui::text(std::string{type_text[i]}) | ftxui::color(colors[(int)text_colors[i]]);
        if(text_colors[i] == TypingState::WRONG)
            el |= ftxui::underlined;
        if(i == cursor)
            el |= ftxui::bgcolor(ftxui::Color::White );
        elements[i] = el;
    }
    return elements;
}

int main(int argc, char** argv) {
    tts::TypingSpeedTerminal terminal;
    terminal.start();
    //ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    //ftxui::Component button = ftxui::Button("Stop Game", screen.ExitLoopClosure());
    ////auto renderer = ftxui::Renderer(button);
    //screen.Loop(button);
//
    //ftxui::Component button2 = ftxui::Button("Now actually close it", screen.ExitLoopClosure());
    //screen.Loop(button2);
//
    //screen.Loop(renderer);
    /* Print text to center of terminal
     *
    ftxui::Screen screen = ftxui::Screen::Create(ftxui::Dimension::Full(), ftxui::Dimension::Full());

    // Text inside a flexbox, display in center of screen
    ftxui::Element text = ftxui::text("This is a test text to be typed, go on!") | ftxui::color(ftxui::Color::GrayDark);
    ftxui::Element box = ftxui::flexbox({text}, ftxui::FlexboxConfig().Set(ftxui::FlexboxConfig::JustifyContent::Center).Set(ftxui::FlexboxConfig::AlignContent::Center));

    // render elements onto the screen
    ftxui::Render(screen, box);
    // actually print the screen to stdout
    screen.Print();
    */

    //ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    //std::string input_user;
    //ftxui::Component input = ftxui::Input(&input_user);
    //input |= ftxui::CatchEvent([&](ftxui::Event event) {
    //    return event.is_character() && input_user.length() >= type_text.length();
    //});
//
//
    //ftxui::Component com = ftxui::Container::Horizontal({
    //    input,
    //});
//
    //auto renderer = ftxui::Renderer(input, [&] {
    //    return ftxui::flexbox({
    //        ftxui::window(ftxui::text("Timer: 1min"),ftxui::hbox(text_colored(input_user))),
    //    }, ftxui::FlexboxConfig()
    //    .Set(ftxui::FlexboxConfig::AlignContent::Center)
    //    .Set(ftxui::FlexboxConfig::JustifyContent::Center));
    //});
//
    //screen.Loop(renderer);
    //screen.Exit();
    //return 0;
}
