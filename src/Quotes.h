#ifndef TYPINGSPEEDTERMINAL_QUOTES_H
#define TYPINGSPEEDTERMINAL_QUOTES_H

#include <string>
#include <nlohmann/json.hpp>

namespace tts {

    class Quotes {
    protected:
        /** Singleton storing quotes parsed from quotes-file */
        inline static nlohmann::json _quotes;
        /** Singleton storing movies an the amount of quotes within this tag */
        inline static std::vector<std::tuple<std::string, int>> _tags;
        /** Filter quotes with these tags */
        inline static std::vector<std::string> _filter;
        /**
         * Parse the .json containing all quotes
         * @param filename Path to quotes.json
         * @return Parsed JSON-Object
         */
        static nlohmann::json _fetch_quotes_from_file(const std::string& filename);
        /** Get all available tags including the amount of quotes within this tag */
        static std::vector<std::tuple<std::string, int>> _fetch_tags_from_quotes();
    public:
        /**
         * Get a random quote. Use tags to filter quotes.
         * Empty result if tags do not exist.
         * @return Random quote
         */
        static std::tuple<std::string, std::string> quote();
        /**
         * Get a parsed json object with all movies and quotes
         * @return json-object with quotes categorized by movie
         */
        static nlohmann::json& quotes();
        /**
         * Get all available tags for quotes.
         * @return List of available tags in form (movie, number quotes)
         */
        static std::vector<std::tuple<std::string, int>> tags();
        /**
         * Set tags to filter for, when calling 'Quote::quote()'
         * @param filter_tags Tags to filter for
         */
        static void filter(const std::vector<std::string>& filter_tags);
    };

} // tts

#endif //TYPINGSPEEDTERMINAL_QUOTES_H
