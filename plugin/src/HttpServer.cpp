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
#include "ConfigManager.hpp"
#include "Errors.hpp"
#include "EventMapper.hpp"
#include "HandleManager.hpp"
#include "MainThreadDispatch.hpp"
#include "SSE.hpp"
#include "endpoints/handwritten/DemoEndpoints.hpp"
#include "endpoints/handwritten/TextEndpoints.hpp"
#include "endpoints/handwritten/XMPEndpoints.hpp"
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
std::vector<RouteEntry> HttpServer::customRoutes_;
std::vector<PatternRouteEntry> HttpServer::patternRoutes_;

// Server instance pointer (for shutdown)
static std::unique_ptr<httplib::Server> gServer;
static std::mutex gServerMutex;

/*******************************************************************************
 * Route Registration Functions
 ******************************************************************************/

void HttpServer::RegisterRoute(HttpMethod method, const std::string &path,
                               RouteHandler handler) {
  customRoutes_.push_back({method, path, handler});
}

void HttpServer::Get(const std::string &path, RouteHandler handler) {
  RegisterRoute(HttpMethod::GET, path, handler);
}

void HttpServer::Post(const std::string &path, RouteHandler handler) {
  RegisterRoute(HttpMethod::POST, path, handler);
}

void HttpServer::Delete(const std::string &path, RouteHandler handler) {
  RegisterRoute(HttpMethod::DELETE, path, handler);
}

void HttpServer::GetWithPattern(const std::string &pattern,
                                PatternRouteHandler handler) {
  patternRoutes_.push_back({HttpMethod::GET, pattern, handler});
}

void HttpServer::PostWithPattern(const std::string &pattern,
                                 PatternRouteHandler handler) {
  patternRoutes_.push_back({HttpMethod::POST, pattern, handler});
}

void HttpServer::DeleteWithPattern(const std::string &pattern,
                                   PatternRouteHandler handler) {
  patternRoutes_.push_back({HttpMethod::DELETE, pattern, handler});
}

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

  // -------------------------------------------------------------------------
  // Configuration Endpoints
  // -------------------------------------------------------------------------

  // GET /config - Get current configuration
  svr.Get("/config", [](const httplib::Request &, httplib::Response &res) {
    json response = {{"success", true},
                     {"config", ConfigManager::Instance().GetConfig()}};
    res.set_content(response.dump(), "application/json");
  });

  // POST /config/port - Change server port
  svr.Post("/config/port", [](const httplib::Request &req,
                              httplib::Response &res) {
    try {
      auto body = json::parse(req.body);

      if (!body.contains("port")) {
        json errorResponse = {{"success", false},
                              {"error", "Missing required field: 'port'"}};
        res.status = 400;
        res.set_content(errorResponse.dump(), "application/json");
        return;
      }

      int newPort = body["port"].get<int>();

      // Validate port range
      if (newPort < ConfigManager::MIN_PORT ||
          newPort > ConfigManager::MAX_PORT) {
        json errorResponse = {
            {"success", false},
            {"error", "Port must be between " +
                          std::to_string(ConfigManager::MIN_PORT) + " and " +
                          std::to_string(ConfigManager::MAX_PORT)}};
        res.status = 400;
        res.set_content(errorResponse.dump(), "application/json");
        return;
      }

      int oldPort = ConfigManager::Instance().GetPort();

      // Save new port to config
      ConfigManager::Instance().SetPort(newPort);
      ConfigManager::Instance().Save();

      // Prepare response before scheduling restart
      json response = {
          {"success", true},
          {"previousPort", oldPort},
          {"newPort", newPort},
          {"message", "Server restarting on port " + std::to_string(newPort)}};
      res.set_content(response.dump(), "application/json");

      // Schedule server restart after response is sent
      // Use a short delay so the HTTP response can complete
      // Schedule server restart after response is sent
      // Use a static thread to avoid detached thread hazards
      static std::thread restartThread;
      if (restartThread.joinable()) {
        restartThread.join();
      }
      restartThread = std::thread([newPort]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        HttpServer::Stop();
        HttpServer::Start(newPort);
      });

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

  // -------------------------------------------------------------------------
  // Custom Routes - Registered by downstream plugins via RegisterRoute()
  // IMPORTANT: Must be registered BEFORE the generic suite dispatcher
  // so specific paths like /doc/info don't match /{suite}/{method}
  // -------------------------------------------------------------------------
  for (const auto &route : customRoutes_) {
    auto handler = [route](const httplib::Request &req,
                           httplib::Response &res) {
      try {
        std::string result = route.handler(req.body);
        res.set_content(result, "application/json");
      } catch (const std::exception &e) {
        json errorResponse = {{"success", false}, {"error", e.what()}};
        res.status = 500;
        res.set_content(errorResponse.dump(), "application/json");
      }
    };

    switch (route.method) {
    case HttpMethod::GET:
      svr.Get(route.path, handler);
      break;
    case HttpMethod::POST:
      svr.Post(route.path, handler);
      break;
    case HttpMethod::PUT:
      svr.Put(route.path, handler);
      break;
    case HttpMethod::DELETE:
      svr.Delete(route.path, handler);
      break;
    case HttpMethod::PATCH:
      svr.Patch(route.path, handler);
      break;
    }
  }

  // -------------------------------------------------------------------------
  // Pattern Routes - Routes with path parameters (regex capture groups)
  // Default pattern is ([^/]+) which matches any non-slash characters.
  // Routes can override with a custom pattern via pathParams.pattern in
  // routes.json, e.g. ([a-zA-Z0-9_.-]+) for alphanumeric IDs.
  // -------------------------------------------------------------------------
  for (const auto &route : patternRoutes_) {
    auto handler = [route](const httplib::Request &req,
                           httplib::Response &res) {
      try {
        // Extract captured groups from regex matches
        std::vector<std::string> params;
        // matches[0] is the full match, [1], [2], etc. are capture groups
        for (size_t i = 1; i < req.matches.size(); ++i) {
          params.push_back(std::string(req.matches[i]));
        }

        std::string result = route.handler(req.body, params);
        res.set_content(result, "application/json");
      } catch (const std::exception &e) {
        json errorResponse = {{"success", false}, {"error", e.what()}};
        res.status = 500;
        res.set_content(errorResponse.dump(), "application/json");
      }
    };

    switch (route.method) {
    case HttpMethod::GET:
      svr.Get(route.pattern, handler);
      break;
    case HttpMethod::POST:
      svr.Post(route.pattern, handler);
      break;
    case HttpMethod::DELETE:
      svr.Delete(route.pattern, handler);
      break;
    default:
      // PUT and PATCH not commonly used with path params, add if needed
      break;
    }
  }

  // Suite-specific endpoints (generic catch-all - MUST be after custom routes)
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

  // -------------------------------------------------------------------------
  // Server-Sent Events (SSE) - Real-time push notifications
  // -------------------------------------------------------------------------
  SSE::SetupEndpoint(svr);

  // -------------------------------------------------------------------------
  // Demo Endpoints - Hand-written endpoints for real SDK integration
  // -------------------------------------------------------------------------

  // GET /demo/document-info - Get current document information
  svr.Get("/demo/document-info",
          [](const httplib::Request &, httplib::Response &res) {
            json result = MainThreadDispatch::Run(
                []() -> json { return DemoEndpoints::GetDocumentInfo(); });
            res.set_content(result.dump(), "application/json");
          });

  // GET /demo/layers - List all layers in the document
  svr.Get("/demo/layers", [](const httplib::Request &, httplib::Response &res) {
    json result = MainThreadDispatch::Run(
        []() -> json { return DemoEndpoints::GetLayers(); });
    res.set_content(result.dump(), "application/json");
  });

  // GET /demo/selection - Get information about selected art
  svr.Get("/demo/selection",
          [](const httplib::Request &, httplib::Response &res) {
            json result = MainThreadDispatch::Run(
                []() -> json { return DemoEndpoints::GetSelection(); });
            res.set_content(result.dump(), "application/json");
          });

  // POST /demo/create-rectangle - Create a rectangle in the document
  svr.Post("/demo/create-rectangle", [](const httplib::Request &req,
                                        httplib::Response &res) {
    try {
      json params = req.body.empty() ? json::object() : json::parse(req.body);
      json result = MainThreadDispatch::Run([&params]() -> json {
        return DemoEndpoints::CreateRectangle(params);
      });
      res.set_content(result.dump(), "application/json");
    } catch (const json::parse_error &e) {
      json errorResponse = {
          {"success", false},
          {"error", "Invalid JSON: " + std::string(e.what())}};
      res.status = 400;
      res.set_content(errorResponse.dump(), "application/json");
    }
  });

  // -------------------------------------------------------------------------
  // Custom Routes - Registered by downstream plugins via RegisterRoute()
  // -------------------------------------------------------------------------
  for (const auto &route : customRoutes_) {
    auto handler = [route](const httplib::Request &req,
                           httplib::Response &res) {
      try {
        std::string result = route.handler(req.body);
        res.set_content(result, "application/json");
      } catch (const std::exception &e) {
        json errorResponse = {{"success", false}, {"error", e.what()}};
        res.status = 500;
        res.set_content(errorResponse.dump(), "application/json");
      }
    };

    switch (route.method) {
    case HttpMethod::GET:
      svr.Get(route.path, handler);
      break;
    case HttpMethod::POST:
      svr.Post(route.path, handler);
      break;
    case HttpMethod::PUT:
      svr.Put(route.path, handler);
      break;
    case HttpMethod::DELETE:
      svr.Delete(route.path, handler);
      break;
    case HttpMethod::PATCH:
      svr.Patch(route.path, handler);
      break;
    }
  }
}

/*******************************************************************************
 * HttpServer::ServerThread
 ******************************************************************************/

void HttpServer::ServerThread() {
  // Register all custom routes from generated code (populates customRoutes_/patternRoutes_).
  // This now handles ALL routes including text and XMP endpoints, which were previously
  // registered separately via TextEndpoints::RegisterRoutes() and XMPEndpoints::RegisterRoutes().
  extern void RegisterCustomRoutes();
  RegisterCustomRoutes();

  // DEPRECATED: Text and XMP routes are now registered by CustomRouteRegistration.cpp
  // (generated from routes.json). These calls are commented out to avoid duplicate
  // route registration. Remove entirely once the generated registration is confirmed
  // working in production.
  // TextEndpoints::RegisterRoutes();
  // XMPEndpoints::RegisterRoutes();

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
