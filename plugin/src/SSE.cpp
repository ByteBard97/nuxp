/**
 * Server-Sent Events (SSE) Implementation
 *
 * Based on Flora's SSE implementation, adapted for NUXP.
 * Provides real-time push notifications from plugin to frontend.
 */

#include "SSE.hpp"

#include <chrono>
#include <sstream>

/*******************************************************************************
 * SSE Singleton
 ******************************************************************************/

SSE &SSE::Instance() {
  static SSE instance;
  return instance;
}

/*******************************************************************************
 * SSE Static Methods (delegate to singleton)
 ******************************************************************************/

void SSE::SetupEndpoint(httplib::Server &server) {
  server.Get("/events/stream", [](const httplib::Request &,
                                  httplib::Response &res) {
    // Create a new connection
    auto conn = std::make_shared<SSEConnection>();
    conn->id = Instance().nextConnectionId_++;
    Instance().addConnection(conn);

    // Set up SSE headers
    res.set_header("Content-Type", "text/event-stream");
    res.set_header("Cache-Control", "no-cache");
    res.set_header("Connection", "keep-alive");
    res.set_header("Access-Control-Allow-Origin", "*");

    // Use chunked content provider for streaming
    res.set_chunked_content_provider(
        "text/event-stream",
        [conn](size_t /*offset*/, httplib::DataSink &sink) {
          // Wait for messages or timeout
          std::unique_lock<std::mutex> lock(conn->mutex);

          // Wait up to 15 seconds for a message
          auto hasMessage = conn->cv.wait_for(
              lock, std::chrono::seconds(15),
              [&conn] { return !conn->messageQueue.empty() || !conn->active; });

          // Check if connection was closed
          if (!conn->active) {
            return false; // End the stream
          }

          // Send all queued messages
          if (hasMessage && !conn->messageQueue.empty()) {
            while (!conn->messageQueue.empty()) {
              std::string msg = std::move(conn->messageQueue.front());
              conn->messageQueue.pop();
              sink.write(msg.c_str(), msg.size());
            }
          } else {
            // Send heartbeat on timeout
            std::string heartbeat = ":heartbeat\n\n";
            sink.write(heartbeat.c_str(), heartbeat.size());
          }

          return true; // Continue streaming
        },
        [conn](bool /*success*/) {
          // Cleanup when connection closes
          conn->active = false;
          conn->cv.notify_all();
          Instance().removeConnection(conn->id);
        });
  });

  // Diagnostics endpoint for SSE connections
  server.Get("/diagnostics/sse",
             [](const httplib::Request &, httplib::Response &res) {
               json response = {{"success", true},
                                {"connections", SSE::ConnectionCount()}};
               res.set_content(response.dump(), "application/json");
             });
}

void SSE::Broadcast(const std::string &eventType, const json &data) {
  std::ostringstream oss;
  oss << "event: " << eventType << "\n";
  oss << "data: " << data.dump() << "\n\n";
  Instance().broadcast(oss.str());
}

void SSE::BroadcastRaw(const std::string &message) {
  Instance().broadcast(message);
}

void SSE::SendHeartbeat() {
  Instance().broadcast(":heartbeat\n\n");
}

size_t SSE::ConnectionCount() {
  return Instance().connectionCount();
}

void SSE::CloseAll() {
  Instance().closeAll();
}

/*******************************************************************************
 * SSE Internal Implementation
 ******************************************************************************/

void SSE::addConnection(std::shared_ptr<SSEConnection> conn) {
  std::lock_guard<std::mutex> lock(connectionsMutex_);
  connections_.push_back(conn);
}

void SSE::removeConnection(int connId) {
  std::lock_guard<std::mutex> lock(connectionsMutex_);
  connections_.erase(
      std::remove_if(connections_.begin(), connections_.end(),
                     [connId](const std::shared_ptr<SSEConnection> &c) {
                       return c->id == connId;
                     }),
      connections_.end());
}

void SSE::broadcast(const std::string &message) {
  std::lock_guard<std::mutex> lock(connectionsMutex_);
  for (auto &conn : connections_) {
    if (conn->active) {
      std::lock_guard<std::mutex> connLock(conn->mutex);
      conn->messageQueue.push(message);
      conn->cv.notify_one();
    }
  }
}

size_t SSE::connectionCount() {
  std::lock_guard<std::mutex> lock(connectionsMutex_);
  return connections_.size();
}

void SSE::closeAll() {
  std::lock_guard<std::mutex> lock(connectionsMutex_);
  for (auto &conn : connections_) {
    conn->active = false;
    conn->cv.notify_all();
  }
  connections_.clear();
}

/*******************************************************************************
 * Event Emitters
 ******************************************************************************/

namespace Events {

void EmitSelectionChange(const std::vector<int> &selectedIds, int count) {
  json data = {{"selectedIds", selectedIds}, {"count", count}};
  SSE::Broadcast("selection", data);
}

void EmitDocumentEvent(const std::string &eventType,
                       const std::string &documentName) {
  // Escape special characters in document name for JSON safety
  json data = {{"type", eventType}, {"documentName", documentName}};
  SSE::Broadcast("document", data);
}

void EmitLayerChange(int layerCount) {
  json data = {{"layerCount", layerCount}};
  SSE::Broadcast("layers", data);
}

void EmitPluginEvent(const std::string &eventType, const json &data) {
  SSE::Broadcast(eventType, data);
}

void EmitVersion(const std::string &version) {
  json data = {{"version", version}};
  SSE::Broadcast("version", data);
}

} // namespace Events

