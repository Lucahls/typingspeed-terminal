//
// Created by Luca Hessels on 21.06.24.
//

#include "Quotes.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace tts {
    std::string Quotes::_fetch_quote() {
        cpr::Response r = cpr::Get(
                cpr::Url{"https://api.quotable.io/random"});
        return nlohmann::json::parse(r.text)["content"];
    }

    std::string Quotes::quote() {
        return _fetch_quote();
    }
} // tts