#pragma once
#include <string>
#include <unordered_map>

std::string get_user_config_path();
void ensure_config_exists();
std::unordered_map<std::string, std::string> read_config(const std::string& path);
std::string prompt_and_insert_api_key(const std::string& config_path);
void set_api_key(const std::string& config_path, const std::string& api_key);