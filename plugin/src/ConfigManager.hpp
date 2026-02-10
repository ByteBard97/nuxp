#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <mutex>

/**
 * ConfigManager - Persistent configuration for NUXP plugin
 *
 * Manages configuration stored in a JSON file:
 *   macOS: ~/Library/Application Support/NUXP/config.json
 *   Windows: %APPDATA%/NUXP/config.json
 *
 * Thread-safe singleton pattern.
 */
class ConfigManager {
public:
    // Default port is set via CMake: NUXP_DEFAULT_PORT
    // Fallback to 8080 if not defined
#ifndef NUXP_DEFAULT_PORT
#define NUXP_DEFAULT_PORT 8080
#endif
    static constexpr int DEFAULT_PORT = NUXP_DEFAULT_PORT;
    static constexpr int MIN_PORT = 1024;
    static constexpr int MAX_PORT = 65535;

    /**
     * Get singleton instance
     */
    static ConfigManager& Instance();

    /**
     * Load configuration from disk.
     * Creates default config if file doesn't exist.
     */
    void Load();

    /**
     * Save current configuration to disk.
     * Creates config directory if it doesn't exist.
     */
    void Save();

    /**
     * Get the configured server port.
     * @return Port number (default: 8080)
     */
    int GetPort() const;

    /**
     * Set the server port.
     * Does NOT automatically restart the server - caller must do that.
     * @param port New port number (must be between MIN_PORT and MAX_PORT)
     * @return true if port was valid and set, false otherwise
     */
    bool SetPort(int port);

    /**
     * Get the full config as JSON (for /config endpoint)
     */
    nlohmann::json GetConfig() const;

    /**
     * Get the config directory path (platform-specific)
     */
    static std::string GetConfigDir();

    /**
     * Get the config file path
     */
    static std::string GetConfigPath();

    // Delete copy/move constructors
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

private:
    ConfigManager() = default;

    void CreateDefaultConfig();

    mutable std::mutex mutex_;
    nlohmann::json config_;
};
