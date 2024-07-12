#ifndef TYPINGSPEEDTERMINAL_FRAME_H
#define TYPINGSPEEDTERMINAL_FRAME_H

#include <ftxui/component/component.hpp>
#include "TypingSpeedTerminal.h"
#include "Timer.h"

namespace tts {
    class TypingSpeedTerminal;

    /** Possible states for characters when typed */
    enum class TypingState {
        CORRECT = 0,
        WRONG,
        EMPTY,
    };

    /** Stats collected while typing */
    struct TypingStats {
        // Keystrokes done
        int correct_keystrokes = 0;
        int wrong_keystrokes   = 0;
        // Characters in sentence
        int correct            = 0;
        int mistakes           = 0;
        int corrected_mistakes = 0;
    };
}

namespace tts::Frames {

    /**
     * Frame Baseclass to render on screen
     */
    class Frame {
    protected:
        tts::TypingSpeedTerminal* _terminal;
    public:
        /**
         * @param terminal Pointer to the Terminal the Frame is rendered from
         */
        explicit Frame(tts::TypingSpeedTerminal* terminal) : _terminal(terminal) {};
        /**
         * Return a ftxui::Component that describes how this Frame is rendered
         * @return ftxui::Component
         */
        virtual  ftxui::Component render() = 0;
        virtual ~Frame() = default;
    };

    /** --------------------
     * Derived Frames actually rendering something
     -------------------- */

    /**
     * Start up screen
     */
    class Home : public Frame {
    protected:
        ftxui::Component _button_start;
        ftxui::Component _button_config;
        ftxui::Component _button_quit;
    public:
        explicit         Home(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };


    /**
     * Configuration screen
     */
    class Config : public Frame {
    protected:
        ftxui::Component                          _button_back;
        /** Tags (Movies) that can be selected via checkboxes */
        std::vector<std::tuple<std::string, int>> _tags;
        ftxui::Component                          _tags_checkbox;
        std::unique_ptr<bool[]>                   _tags_states;
        std::vector<std::string>                  _selected_tags;
        /** Write the selected tags to Quotes configuration */
        void                                      _save_tags();
    public:
        explicit         Config(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
    };


    /**
     * Typing-Test screen
     */
    class TypingTerminal : public Frame {
    protected:
        int                                  _seconds = 15;
        Timer                                _timer;
        /** Currently inputted characters by the user */
        std::string                          _input;
        /** Displayed Input, handling user input */
        ftxui::Component                     _input_field;
        /** Position of the cursor in the input */
        int                                  _input_index = 0;
        /** Text that has to be typed for the test */
        std::string                          _typing_text;
        /** Correctness for each character that has to be typed */
        std::vector<tts::TypingState>        _typing_states;
        /** Quote of form (quote, author) */
        std::tuple<std::string, std::string> _quote;
        bool                                 _is_over = false;
        /**
         * compare a typed text with the expected text that needs to be entered
         * @param text Text must be typed
         * @param verify Text that was typed
         * @return Typing-states for each character in the 'verify'-text
         */
        static std::vector<tts::TypingState> _check_text(std::string const &text, std::string const &verify);
        /**
         * Generate a colored text, each color indicating the correctness of the text
         * @param text Text that was typed
         * @return Colored text
         */
        std::vector<ftxui::Element>          _generate_colored_text(const std::string &text);
        /** Keep statistics for each keystrokes */
        void                                 _keep_statistics_keys(const ftxui::Event& input);
        /** Keep statistics for each character of each typed sentence */
        void                                 _keep_statistics_chars();
        /** Load the next quote */
        void                                 _next();
    public:
        explicit TypingTerminal(tts::TypingSpeedTerminal* terminal);
        ftxui::Component render() override;
        TypingStats stats = TypingStats();
    };


    /**
     * End screen with stats
     */
    class Stats : public Frame {
    protected:
        ftxui::Component _button_restart;
        ftxui::Component _button_config;
        ftxui::Component _button_quit;
        /** Resulting statistics */
        TypingStats      _stats;
        int              _wpm;
        int              _accuracy;
    public:
        explicit Stats(tts::TypingSpeedTerminal* terminal, TypingStats& stats, int seconds);
        ftxui::Component render() override;
    };
}

#endif //TYPINGSPEEDTERMINAL_FRAME_H
