#pragma once
#include <string>

std::string fetch_weather(const std::string& city, const std::string& units, const std::string& api_key);

std::string format_weather(const std::string& raw_json, const std::string& units);
