#pragma once
#include <string>

bool is_cache_valid(const std::string& city, const std::string& units, int max_age_minutes);
std::string read_cache(const std::string& city, const std::string& units);
void write_cache(const std::string& city, const std::string& units, const std::string& data);
