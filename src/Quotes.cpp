#include "Quotes.h"

#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fstream>
#include <random>

namespace tts {
    nlohmann::json& Quotes::quotes() {
        if(_quotes.is_null())
            _quotes = _fetch_quotes_from_file("../quotes.json");
        return _quotes;
    }

    nlohmann::json Quotes::_fetch_quotes_from_file(const std::string& path) {
        std::ifstream file(path);
        return nlohmann::json::parse(file);
    }

    std::vector<std::tuple<std::string, int>> Quotes::tags(){
        if(_tags.empty())
            _tags = _fetch_tags_from_quotes();
        return _tags;
    }

    std::vector<std::tuple<std::string, int>> Quotes::_fetch_tags_from_quotes() {
        std::vector<std::tuple<std::string, int>> tags;
        for(auto& [movie, quotes] : quotes().items())
            tags.push_back({movie, quotes.size()});
        return tags;
    }

    std::tuple<std::string, std::string> Quotes::quote() {
        std::vector<std::string> filter;
        if(!_filter.empty())
            filter = _filter;
        else {
            // Extract only the movie name from the tags
            std::vector<std::string> movies(tags().size());
            std::transform(tags().begin(), tags().end(), movies.begin(),
                           [](const std::tuple<std::string, int> &tuple) {
                return std::get<0>(tuple);
            });
            filter = movies;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_movie(0, filter.size() - 1);
        std::string movie = filter[dist_movie(gen)];

        std::uniform_int_distribution<> dist_quote(0, quotes()[movie].size() - 1);
        int quote = dist_quote(gen);

        return {quotes()[movie][quote]["quote"], quotes()[movie][quote]["author"]};
    }

    void Quotes::filter(const std::vector<std::string> &filter_tags) {
        _filter = filter_tags;
    }


} // tts