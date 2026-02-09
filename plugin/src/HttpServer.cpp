/**
 * NUXP HTTP Server Implementation
 *
 * Provides an HTTP/JSON API for external applications to communicate
 * with the Adobe Illustrator plugin. Uses cpp-httplib for HTTP handling.
 *
 * All SDK calls are dispatched to the main thread via MainThreadDispatch
 * to ensure thread safety.
 */

#include "HttpServer.hpp"
#include "Errors.hpp"
#include "EventMapper.hpp"
#include "HandleManager.hpp"
#include "MainThreadDispatch.hpp"
#include "endpoints/generated/CentralDispatcher.h"

// cpp-httplib (header-only HTTP library)
#include "httplib.h"

// nlohmann/json (header-only JSON library)
#include <nlohmann/json.hpp>

#include <chrono>
#include <memory>

using json = nlohmann::json;

/*******************************************************************************
 * Static Member Definitions
 ******************************************************************************/

std::thread HttpServer::serverThread_;
std::atomic<bool> HttpServer::running_{false};
std::atomic<bool> HttpServer::ready_{false};
int HttpServer::port_ = 8080;

// Server instance pointer (for shutdown)
static std::unique_ptr<httplib::Server> gServer;
static std::mutex gServerMutex;

/*******************************************************************************
 * HttpServer::Start
 ******************************************************************************/

void HttpServer::Start(int port) {
  // Don't start if already running
  if (running_.load()) {
    return;
  }

  port_ = port;
  running_.store(true);
  ready_.store(false);

  // Launch server thread
  serverThread_ = std::thread(ServerThread);

  // Wait for server to be ready (with timeout)
  auto startTime = std::chrono::steady_clock::now();
  while (!ready_.load() && running_.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Timeout after 5 seconds
    auto elapsed = std::chrono::steady_clock::now() - startTime;
    if (elapsed > std::chrono::seconds(5)) {
      break;
    }
  }
}

/*******************************************************************************
 * HttpServer::Stop
 ******************************************************************************/

void HttpServer::Stop() {
  if (!running_.load()) {
    return;
  }

  // Signal shutdown
  running_.store(false);

  // Stop the server to unblock listen()
  {
    std::lock_guard<std::mutex> lock(gServerMutex);
    if (gServer) {
      gServer->stop();
    }
  }

  // Wait for thread to finish
  if (serverThread_.joinable()) {
    serverThread_.join();
  }

  // Clean up server instance
  {
    std::lock_guard<std::mutex> lock(gServerMutex);
    gServer.reset();
  }

  ready_.store(false);
}

/*******************************************************************************
 * HttpServer::IsRunning
 ******************************************************************************/

bool HttpServer::IsRunning() { return running_.load() && ready_.load(); }

/*******************************************************************************
 * HttpServer::GetPort
 ******************************************************************************/

int HttpServer::GetPort() { return port_; }

/*******************************************************************************
 * HttpServer::GetBaseUrl
 ******************************************************************************/

std::string HttpServer::GetBaseUrl() {
  return "http://localhost:" + std::to_string(port_);
}

/*******************************************************************************
 * HttpServer::ConfigureRoutes
 ******************************************************************************/

void HttpServer::ConfigureRoutes() {
  std::lock_guard<std::mutex> lock(gServerMutex);
  if (!gServer) {
    return;
  }

  auto &svr = *gServer;

  // CORS headers for browser access from Tauri app
  svr.set_default_headers(
      {{"Access-Control-Allow-Origin", "*"},
       {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
       {"Access-Control-Allow-Headers", "Content-Type, Authorization"}});

  // Handle CORS preflight requests
  svr.Options(R"(.*)", [](const httplib::Request &, httplib::Response &res) {
    res.status = 204;
  });

  // Health check endpoint
  svr.Get("/health", [](const httplib::Request &, httplib::Response &res) {
    json response = {{"success", true},
                     {"status", "ok"},
                     {"plugin", "NUXP"},
                     {"version", NUXP_VERSION}};
    res.set_content(response.dump(), "application/json");
  });

  // Plugin info endpoint
  svr.Get("/info", [](const httplib::Request &, httplib::Response &res) {
    json response = {{"success", true},
                     {"plugin",
                      {{"name", "NUXP"},
                       {"version", NUXP_VERSION},
                       {"description", "Illustrator HTTP/JSON Bridge"}}},
                     {"handles",
                      {{"art", HandleManager::art.Size()},
                       {"layers", HandleManager::layers.Size()},
                       {"documents", HandleManager::documents.Size()}}}};
    res.set_content(response.dump(), "application/json");
  });

  // Generic API call endpoint
  // POST /api/call { "suite": "AIArt", "method": "NewArt", "args": {...} }
  svr.Post(
      "/api/call", [](const httplib::Request &req, httplib::Response &res) {
        try {
          auto body = json::parse(req.body);

          // Validate required fields
          if (!body.contains("suite") || !body.contains("method")) {
            json errorResponse = {
                {"success", false},
                {"error", "Missing required fields: 'suite' and 'method'"}};
            res.status = 400;
            res.set_content(errorResponse.dump(), "application/json");
            return;
          }

          std::string suite = body["suite"].get<std::string>();
          std::string method = body["method"].get<std::string>();
          json args = body.value("args", json::object());

          // Dispatch to main thread and wait for result
          json result = MainThreadDispatch::Run([&]() -> json {
            try {
              // Route to the appropriate suite wrapper via CentralDispatcher
              return Flora::Dispatch(suite, method, args);
            } catch (const std::exception &e) {
              return json{{"success", false},
                          {"error", e.what()},
                          {"suite", suite},
                          {"method", method}};
            }
          });

          res.set_content(result.dump(), "application/json");

        } catch (const json::parse_error &e) {
          json errorResponse = {
              {"success", false},
              {"error", "Invalid JSON: " + std::string(e.what())}};
          res.status = 400;
          res.set_content(errorResponse.dump(), "application/json");
        } catch (const std::exception &e) {
          json errorResponse = {{"success", false}, {"error", e.what()}};
          res.status = 500;
          res.set_content(errorResponse.dump(), "application/json");
        }
      });

  // Suite-specific endpoints
  // Pattern: POST /{SuiteName}/{MethodName}
  // Example: POST /AIArt/NewArt { "type": 1, "paintOrder": 1 }
  svr.Post(R"(/(\w+)/(\w+))", [](const httplib::Request &req,
                                 httplib::Response &res) {
    try {
      std::string suite = std::string(req.matches[1]);
      std::string method = std::string(req.matches[2]);
      json args = req.body.empty() ? json::object() : json::parse(req.body);

      // Dispatch to main thread
      json result = MainThreadDispatch::Run([&]() -> json {
        try {
          // Route to the appropriate suite wrapper via CentralDispatcher
          return Flora::Dispatch(suite, method, args);
        } catch (const std::exception &e) {
          return json{{"success", false},
                      {"error", e.what()},
                      {"suite", suite},
                      {"method", method}};
        }
      });

      res.set_content(result.dump(), "application/json");

    } catch (const json::parse_error &e) {
      json errorResponse = {
          {"success", false},
          {"error", "Invalid JSON: " + std::string(e.what())}};
      res.status = 400;
      res.set_content(errorResponse.dump(), "application/json");
    } catch (const std::exception &e) {
      json errorResponse = {{"success", false}, {"error", e.what()}};
      res.status = 500;
      res.set_content(errorResponse.dump(), "application/json");
    }
  });

  // Handle invalidation endpoint (for testing)
  svr.Post("/handles/invalidate", [](const httplib::Request &,
                                     httplib::Response &res) {
    MainThreadDispatch::Run([&]() -> json {
      HandleManager::InvalidateAll();
      return json::object();
    });

    json response = {{"success", true}, {"message", "All handles invalidated"}};
    res.set_content(response.dump(), "application/json");
  });

  // Handle stats endpoint
  svr.Get(
      "/handles/stats", [](const httplib::Request &, httplib::Response &res) {
        json response = {{"success", true},
                         {"handles",
                          {{"art", HandleManager::art.Size()},
                           {"layers", HandleManager::layers.Size()},
                           {"documents", HandleManager::documents.Size()}}}};
        res.set_content(response.dump(), "application/json");
      });

  // Events endpoint (Long Polling)
  // Returns immediately if events exist, otherwise waits up to 1s
  svr.Get("/events", [](const httplib::Request &, httplib::Response &res) {
    int timeoutMs = 1000;
    int intervalMs = 50;
    int elapsedMs = 0;

    while (!EventMapper::Instance().HasEvents() && elapsedMs < timeoutMs) {
      std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
      elapsedMs += intervalMs;
    }

    json events = EventMapper::Instance().PopAll();
    json response = {{"success", true}, {"events", events}};
    res.set_content(response.dump(), "application/json");
  });
}

/*******************************************************************************
 * HttpServer::ServerThread
 ******************************************************************************/

void HttpServer::ServerThread() {
  // Create server instance
  {
    std::lock_guard<std::mutex> lock(gServerMutex);
    gServer = std::make_unique<httplib::Server>();
  }

  // Configure routes
  ConfigureRoutes();

  // Attempt to bind to the port
  {
    std::lock_guard<std::mutex> lock(gServerMutex);
    if (!gServer->bind_to_port("localhost", port_)) {
      running_.store(false);
      return;
    }
  }

  // Mark as ready
  ready_.store(true);

  // Start listening (blocks until stop() is called)
  // We need a local pointer since listen_after_bind() blocks and we can't
  // hold the lock during the call
  httplib::Server *serverPtr = nullptr;
  {
    std::lock_guard<std::mutex> lock(gServerMutex);
    if (gServer && running_.load()) {
      serverPtr = gServer.get();
    }
  }

  // listen_after_bind() blocks until stop() is called from another thread
  if (serverPtr && running_.load()) {
    serverPtr->listen_after_bind();
  }

  // Clean up
  ready_.store(false);
}
