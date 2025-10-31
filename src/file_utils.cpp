#include "weather/file_utils.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

namespace fs = std::filesystem;

std::string get_user_config_path() {
    const char* home = std::getenv("HOME");
    if (!home) throw std::runtime_error("HOME not set");
    return std::string(home) + "/.weather/settings.ini";
}

void create_default_config(const std::string& path) {
    std::ofstream config(path);
    config << "[weather]\n";
    config << "units=C\n";
    config << "cache_enabled=true\n";
    config << "default_city=Toronto\n";	
    config << "#OpenWeatherMap API key\n";
    config << "api_key=your_api_key_here\n";  // Leave blank or prompt later
	config << "#End of ini\n";
    config.close();
}

void ensure_config_exists() {
    std::string config_path = get_user_config_path();
    if (!fs::exists(config_path)) {
        fs::create_directories(fs::path(config_path).parent_path());
        create_default_config(config_path);
    }
}


std::unordered_map<std::string, std::string> read_config(const std::string& path) {
    std::unordered_map<std::string, std::string> config;
    config = {
    {"default_city","Toronto"},
    {"units", "C"}    
    };
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            config[key] = value;
        }
    }

    return config;
}

std::string prompt_and_insert_api_key(const std::string& config_path) {
	//prompt for API Key
	std::string api_key;
    std::cout << "Enter your OpenWeatherMap API key: ";
    std::getline(std::cin, api_key);
	
	set_api_key(config_path, api_key);
	
	
	return api_key;

}

void set_api_key(const std::string& config_path, const std::string& api_key) {
	// Read config file
	std::ifstream in(config_path);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string content = buffer.str();
    in.close();	
	
	// Replace the empty api_key line
    size_t pos = content.find("api_key=");
    if (pos != std::string::npos) {
        size_t end = content.find('\n', pos);
        content.replace(pos, end - pos, "api_key=" + api_key);
    }
	
	std::ofstream out(config_path);
    out << content;
    out.close();
}