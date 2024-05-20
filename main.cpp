#include "fmt/core.h"
#include "ftxui/screen/screen.hpp"
#include "ftxui/dom/elements.hpp"

int main(int argc, char** argv) {

    ftxui::Element document =
            ftxui::hbox({
                                ftxui::text("left")   | ftxui::border,
                                ftxui::text("middle") | ftxui::border | ftxui::flex,
                                ftxui::text("right")  | ftxui::border,
                        });
    ftxui::Screen screen = ftxui::Screen::Create(ftxui::Dimension::Fixed(100), ftxui::Dimension::Fit(document));

    ftxui::Render(screen, document);
    screen.Print();

    return 0;
}
