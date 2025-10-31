#include <iostream>
#include <string>
#include <curl/curl.h>
#include "weather/json.hpp"
#include <sstream>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string fetch_weather(const std::string& city, const std::string& units, const std::string& api_key) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string url = "https://api.openweathermap.org/data/2.5/weather?q=" + city +
                          "&appid=" + api_key + "&units=" + units;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

bool is_night(const json& j) {
    if (!j.contains("dt") || !j.contains("sys")) return false;
    long now = j["dt"];
    long sunrise = j["sys"]["sunrise"];
    long sunset = j["sys"]["sunset"];
    return now < sunrise || now > sunset;
}

std::string emoji_for(const std::string& desc, bool night) {
    std::string d = desc;
    std::transform(d.begin(), d.end(), d.begin(), ::tolower);

    if (d.find("clear") != std::string::npos) return night ? "🌙" : "☀️";
    if (d.find("cloud") != std::string::npos) return night ? "☁️" : "⛅";
    if (d.find("rain") != std::string::npos) return "🌧️";
    if (d.find("snow") != std::string::npos) return "🌨️";
    if (d.find("mist") != std::string::npos || d.find("fog") != std::string::npos) return "🌫️";
    if (d.find("thunder") != std::string::npos) return "⛈️";
    return "🌡️";
}

std::string format_weather(const std::string& raw_json, const std::string& units) {
    try {
        auto j = json::parse(raw_json);
        std::string desc = j["weather"][0]["description"];
        double temp = j["main"]["temp"];
        int humidity = j["main"]["humidity"];
		
		bool night = is_night(j);
		std::string icon = emoji_for(desc, night);

        std::ostringstream out;
        out << icon << " Weather in " << j["name"] << ": "
            << temp << "°" << units <<", " << desc << ", "
            << humidity << "% humidity";
        return out.str();
    } catch (...) {
        return "Error: Unable to parse weather data.";
    }
}
