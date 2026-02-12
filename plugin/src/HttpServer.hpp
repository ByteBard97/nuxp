#pragma once

/**
 * NUXP HTTP Server
 *
 * Provides an HTTP/JSON API for external applications to communicate
 * with the Adobe Illustrator plugin. The server runs on a background
 * thread and dispatches SDK calls to the main thread via MainThreadDispatch.
 *
 * Thread Safety:
 * - The HTTP server runs entirely on a background thread
 * - All SDK calls are dispatched to the main thread
 * - The server can be started/stopped from any thread
 *
 * Route Registration:
 * - Downstream plugins can register custom routes via RegisterRoute()
 * - Routes must be registered BEFORE calling Start()
 * - Use RegisterRouteHandler() for simple request -> string response patterns
 */

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <vector>

/**
 * HTTP method types for route registration
 */
enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH
};

/**
 * Handler function type for registered routes.
 * Takes request body (empty for GET), returns response body (JSON string).
 */
using RouteHandler = std::function<std::string(const std::string& body)>;

/**
 * Handler function type for pattern routes with path parameters.
 * Takes request body and a vector of extracted path parameters.
 * For pattern R"(/plant/([a-f0-9-]+))", params[0] would be the UUID.
 */
using PatternRouteHandler = std::function<std::string(const std::string& body,
                                                       const std::vector<std::string>& params)>;

/**
 * Route registration entry
 */
struct RouteEntry {
    HttpMethod method;
    std::string path;
    RouteHandler handler;
};

/**
 * Pattern route registration entry (for routes with path parameters)
 */
struct PatternRouteEntry {
    HttpMethod method;
    std::string pattern;  // Regex pattern, e.g., R"(/plant/([a-f0-9-]+))"
    PatternRouteHandler handler;
};

class HttpServer {
public:
    /**
     * Register a custom route handler.
     * Must be called BEFORE Start().
     *
     * @param method HTTP method (GET, POST, etc.)
     * @param path URL path (e.g., "/plant/place")
     * @param handler Function that takes request body and returns JSON response
     */
    static void RegisterRoute(HttpMethod method, const std::string& path, RouteHandler handler);

    /**
     * Convenience: Register a GET route
     */
    static void Get(const std::string& path, RouteHandler handler);

    /**
     * Convenience: Register a POST route
     */
    static void Post(const std::string& path, RouteHandler handler);

    /**
     * Convenience: Register a DELETE route
     */
    static void Delete(const std::string& path, RouteHandler handler);

    /**
     * Register a GET route with regex pattern for path parameters.
     * Pattern uses regex groups to capture path segments.
     * Example: R"(/plant/([a-f0-9-]+))" captures UUID from /plant/abc-123
     *
     * @param pattern Regex pattern with capture groups
     * @param handler Function receiving body and extracted params
     */
    static void GetWithPattern(const std::string& pattern, PatternRouteHandler handler);

    /**
     * Register a POST route with regex pattern for path parameters.
     */
    static void PostWithPattern(const std::string& pattern, PatternRouteHandler handler);

    /**
     * Register a DELETE route with regex pattern for path parameters.
     */
    static void DeleteWithPattern(const std::string& pattern, PatternRouteHandler handler);

    /**
     * Start the HTTP server on a background thread.
     * If the server is already running, this is a no-op.
     *
     * @param port The port to listen on (default: 8080)
     */
    static void Start(int port = 8080);

    /**
     * Stop the HTTP server and join the background thread.
     * Blocks until the server has fully stopped.
     * Safe to call even if the server is not running.
     */
    static void Stop();

    /**
     * Check if the HTTP server is currently running.
     *
     * @return true if the server is running
     */
    static bool IsRunning();

    /**
     * Get the port the server is listening on.
     *
     * @return The current port number
     */
    static int GetPort();

    /**
     * Get the base URL for the server.
     *
     * @return The base URL (e.g., "http://localhost:8080")
     */
    static std::string GetBaseUrl();

private:
    /**
     * The main server thread function.
     * Sets up routes and starts listening for requests.
     */
    static void ServerThread();

    /**
     * Configure all HTTP routes on the server.
     * Called from ServerThread() before listening.
     */
    static void ConfigureRoutes();

    // Server thread handle
    static std::thread serverThread_;

    // Atomic flag to signal shutdown
    static std::atomic<bool> running_;

    // Atomic flag to signal server is ready
    static std::atomic<bool> ready_;

    // Port number
    static int port_;

    // Registered custom routes (populated before Start())
    static std::vector<RouteEntry> customRoutes_;

    // Registered pattern routes with path parameters (populated before Start())
    static std::vector<PatternRouteEntry> patternRoutes_;
};
