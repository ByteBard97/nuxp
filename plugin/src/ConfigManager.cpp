#include "ConfigManager.hpp"

#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

std::string ConfigManager::GetConfigDir() {
#ifdef _WIN32
    const char* appdata = std::getenv("APPDATA");
    if (appdata) {
        return std::string(appdata) + "\\NUXP";
    }
    return ".";
#else
    const char* home = std::getenv("HOME");
    if (home) {
        return std::string(home) + "/Library/Application Support/NUXP";
    }
    return ".";
#endif
}

std::string ConfigManager::GetConfigPath() {
#ifdef _WIN32
    return GetConfigDir() + "\\config.json";
#else
    return GetConfigDir() + "/config.json";
#endif
}

void ConfigManager::CreateDefaultConfig() {
    config_ = {
        {"version", 1},
        {"server", {
            {"port", DEFAULT_PORT}
        }}
    };
}

void ConfigManager::Load() {
    std::lock_guard<std::mutex> lock(mutex_);

    std::string configPath = GetConfigPath();
    std::ifstream file(configPath);

    if (file.is_open()) {
        try {
            file >> config_;

            // Validate the config has required fields
            if (!config_.contains("server") || !config_["server"].contains("port")) {
                CreateDefaultConfig();
            }
        } catch (const nlohmann::json::exception&) {
            // Corrupted config file - use defaults
            CreateDefaultConfig();
        }
    } else {
        // No config file - create defaults and save
        CreateDefaultConfig();

        // Save will be called separately, but we need to ensure directory exists
        std::string configDir = GetConfigDir();
        MKDIR(configDir.c_str());
    }
}

void ConfigManager::Save() {
    std::lock_guard<std::mutex> lock(mutex_);

    // Ensure config directory exists
    std::string configDir = GetConfigDir();
    MKDIR(configDir.c_str());

    std::string configPath = GetConfigPath();
    std::ofstream file(configPath);

    if (file.is_open()) {
        file << config_.dump(2);  // Pretty print with 2-space indent
    }
}

int ConfigManager::GetPort() const {
    std::lock_guard<std::mutex> lock(mutex_);

    try {
        if (config_.contains("server") && config_["server"].contains("port")) {
            int port = config_["server"]["port"].get<int>();
            // Validate port range
            if (port >= MIN_PORT && port <= MAX_PORT) {
                return port;
            }
        }
    } catch (const nlohmann::json::exception&) {
        // Fall through to default
    }

    return DEFAULT_PORT;
}

bool ConfigManager::SetPort(int port) {
    if (port < MIN_PORT || port > MAX_PORT) {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    if (!config_.contains("server")) {
        config_["server"] = nlohmann::json::object();
    }
    config_["server"]["port"] = port;

    return true;
}

nlohmann::json ConfigManager::GetConfig() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_;
}
