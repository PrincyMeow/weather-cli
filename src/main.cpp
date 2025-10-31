#include <iostream>
#include <string>
#include <unordered_map>
#include "weather/file_utils.hpp"
#include "weather/weather_api.hpp"
#include "weather/json.hpp"
#include "weather/cache_utils.hpp"


int main(int argc, char* argv[]) {
	//check if flag is --help
	if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")){
    std::cout << "Weather CLI Help\n";
	std::cout << "Make sure to set openweathermap.org's API Key in ~/.weather/settings.ini\n";
    std::cout << "Usage:\n";
    std::cout << "  ./weather [--city <name>] [--units C|F] [--api <API Key>] [--refresh|-r]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --city     Set the city name (e.g., Toronto)\n";
    std::cout << "  --units    Set temperature units: C or F\n";
    std::cout << "  --api      Set openweathermap API key \n";
    std::cout << "  --refresh  To ignore cache and gets data from API \n";
    std::cout << "  --help     Show this help message" << std::endl;
    return 0;
	}
	
	//read defaults from settings ini file
	std::string config_path = "";
	try {
        ensure_config_exists();
        config_path = get_user_config_path();
    } catch (const std::exception& e) {
        std::cerr << "Error initializing config: " << e.what() << std::endl;
        return 1;
    }
	//read config file
    auto config = read_config(config_path);
	
	std::string units = config["units"];
	std::string city = config["default_city"];	
	std::string api_key = config["api_key"]; // API Key
	if(api_key == "your_api_key_here" || api_key.empty()){
		api_key = prompt_and_insert_api_key(config_path);
	}
	
	// Parse command-line flags
	bool refresh = false;
	std::string cache_enabled = config["cache_enabled"];
	if(cache_enabled != "true"){
		refresh = true;
	}
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
		
		if (arg == "--city" && i + 1 < argc) {
            city = argv[++i];
        } else if (arg == "--units" && i + 1 < argc) {
            units = argv[++i];
        } else if (arg == "--api" && i + 1 < argc) {
            config["api_key"] = argv[++i];
			set_api_key(config_path, config["api_key"]);
			api_key = config["api_key"];
        } else if (arg == "--refresh" || arg == "-r"){
			refresh = true;
		}else {
            std::cerr << "Unknown flag: " << arg << "\n";
			return 1;
        }
    }	
	
	units = std::toupper(units[0]);
	if (units != "C" && units != "F") {
		std::cerr << "Invalid units: " << units << ". Must be 'C' or 'F'.\n";
    return 1;
	}
	
	//weather api call
	std::string apiunits = (units == "C") ? "metric" : "imperial";
	
	
	std::string response;
	int max_age = 15; // minutes
	if (!refresh && is_cache_valid(city, apiunits, max_age)) {
	//	std::cout << "reading from cache!\n";
		response = read_cache(city, apiunits);
	} else {
	//	std::cout << "making api call\n";
		response = fetch_weather(city, apiunits, api_key);
		write_cache(city, apiunits, response);
	}

	std::string formatted = format_weather(response, units);
	std::cout << formatted << "\n";
    return 0;
}
