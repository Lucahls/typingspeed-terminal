//
// Created by Luca Hessels on 21.06.24.
//

#include "Quotes.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>

namespace tts {
    std::string Quotes::_fetch_quote() {
        std::string base = "https://api.quotable.io/random?maxLength=80";
        std::string request;
        if(!_query_tags.empty())
            request = fmt::format("{}&tags={}", base, _query_tags);
        else
            request = base;

        cpr::Response r = cpr::Get(cpr::Url{request});
        if(r.status_code != 200)
            return "API is down, this is a placeholder";
        return nlohmann::json::parse(r.text)["content"];
    }

    std::string Quotes::quote() {
        return _fetch_quote();
    }

    std::vector<std::string> Quotes::tags(){
        cpr::Response r = cpr::Get(
                cpr::Url{"https://api.quotable.io/tags"});
        if(r.status_code != 200)
            return {};

        std::vector<std::string> tags;
        for(const auto& tag : nlohmann::json::parse(r.text)) {
            std::string name = tag["slug"];
            int count = tag["quoteCount"];
            if(count >= 12)
                tags.push_back(name);
        }
        return tags;
    }

    void Quotes::tags(std::vector<std::string>& tags) {
        _query_tags = fmt::format("{}", fmt::join(tags, "|"));
    }
} // tts