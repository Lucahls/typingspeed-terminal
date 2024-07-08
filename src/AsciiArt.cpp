//
// Created by Luca Hessels on 08.07.24.
//

#include "AsciiArt.h"

namespace tts{

    ftxui::Element to_ascii_art(int number){
        std::string str_num = std::to_string(number);
        std::vector<ftxui::Element> all_digits;
        for(auto digit : str_num){
            all_digits.push_back(to_ascii_art(digit));
        }
        return ftxui::hbox(all_digits);
    }

    ftxui::Element to_ascii_art(char digit){
        int to_int = digit - '0';
        switch (to_int) {
            case 1:
                return ftxui::vbox({
                    ftxui::text("  _   \n"),
                    ftxui::text(" / |  \n"),
                    ftxui::text(" | |  \n"),
                    ftxui::text(" |_|  \n"),
                    });
            case 2:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("|_  ) \n"),
                    ftxui::text(" / /  \n"),
                    ftxui::text("/___| \n"),
                    });
            case 3:
                return ftxui::vbox({
                    ftxui::text(" ____ \n"),
                    ftxui::text("|__ / \n"),
                    ftxui::text(" |_ \\ \n"),
                    ftxui::text("|___/ \n"),
                    });
            case 4:
                return ftxui::vbox({
                    ftxui::text(" _ _  \n"),
                    ftxui::text("| | | \n"),
                    ftxui::text("|_  _|\n"),
                    ftxui::text("  |_| \n"),
                    });
            case 5:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("| __| \n"),
                    ftxui::text("|__ \\ \n"),
                    ftxui::text("|___/ \n"),
                    });
            case 6:
                return ftxui::vbox({
                    ftxui::text("  __ \n"),
                    ftxui::text(" / / \n"),
                    ftxui::text("/ _ \\\n"),
                    ftxui::text("\\___/\n"),
                    });
            case 7:
                return ftxui::vbox({
                    ftxui::text(" ____ \n"),
                    ftxui::text("|__  |\n"),
                    ftxui::text("  / / \n"),
                    ftxui::text(" /_/  \n"),
                    });
            case 8:
                return ftxui::vbox({
                    ftxui::text(" ___  \n"),
                    ftxui::text("( _ ) \n"),
                    ftxui::text("/ _ \\ \n"),
                    ftxui::text("\\___/ \n"),
                    });
            case 9:
                return ftxui::vbox({
                    ftxui::text(" ___ \n"),
                    ftxui::text("/ _ \\\n"),
                    ftxui::text("\\_, /\n"),
                    ftxui::text(" /_/ \n"),
                    });
            case 0:
                return ftxui::vbox({
                    ftxui::text("  __\n"),
                    ftxui::text(" /  \\ \n"),
                    ftxui::text("| () |\n"),
                    ftxui::text(" \\__/ \n"),
                    });
            default:
                return ftxui::text("");
        }
    }

    ftxui::Element title_ascii_art() {
        return ftxui::vbox(
            ftxui::text("  ______            _                                      __\n"),
            ftxui::text(" /_  __/_  ______  (_)___  ____ __________  ___  ___  ____/ /\n"),
            ftxui::text("  / / / / / / __ \\/ / __ \\/ __ `/ ___/ __ \\/ _ \\/ _ \\/ __  / \n"),
            ftxui::text(" / / / /_/ / /_/ / / / / / /_/ (__  ) /_/ /  __/  __/ /_/ /  \n"),
            ftxui::text("/_/  \\__, / .___/_/_/ /_/\\__, /____/ .___/\\___/\\___/\\__,_/   \n"),
            ftxui::text("    /____/_/            /____/    /_/                        "),
            ftxui::hbox(ftxui::text("presented by "),
                        ftxui::text("Luca") | ftxui::hyperlink("https://github.com/Lucahls")
            ) | ftxui::dim | ftxui::color(ftxui::Color::White)
        ) | ftxui::color(ftxui::Color::LightGreen);
    }
}