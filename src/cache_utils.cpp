#include <fstream>
#include <filesystem>
#include <chrono>
#include "weather/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

bool is_cache_valid(const std::string& city, const std::string& units, int max_age_minutes) {
    std::string path = "cache/" + city + "_" + units + ".json";
    if (!fs::exists(path)) return false;

    std::ifstream in(path);
    json j;
    in >> j;

    if (!j.contains("timestamp")) return false;

    long cached_time = j["timestamp"];
    long now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return (now - cached_time) < max_age_minutes * 60;
}

std::string read_cache(const std::string& city, const std::string& units ) {
    std::ifstream in("cache/" + city + "_" + units + ".json");
    json j;
    in >> j;
    return j["data"];
}

void write_cache(const std::string& city, const std::string& units, const std::string& data) {
    fs::create_directory("cache");
    std::ofstream out("cache/" + city + "_" + units + ".json");
    long now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    json j = { {"timestamp", now}, {"data", data} };
    out << j.dump(2);
}
