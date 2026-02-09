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
 */

#include <atomic>
#include <string>
#include <thread>

class HttpServer {
public:
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
};
