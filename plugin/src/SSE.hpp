#pragma once

/**
 * Server-Sent Events (SSE) Implementation
 *
 * Provides real-time push notifications from the C++ plugin to connected
 * frontend clients. Uses httplib's chunked content provider for streaming.
 *
 * Usage:
 *   1. Call SSE::SetupEndpoint(server) during route configuration
 *   2. Use SSE::Broadcast(event, data) to send events to all clients
 *   3. Frontend connects to /events/stream endpoint
 *
 * Thread Safety:
 *   - All public methods are thread-safe
 *   - Connections are managed internally with proper cleanup
 */

#include "httplib.h"
#include <nlohmann/json.hpp>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

using json = nlohmann::json;

/**
 * SSE Connection
 *
 * Represents a single client connection. Each connection has its own
 * message queue and synchronization primitives.
 */
struct SSEConnection {
  int id;
  std::atomic<bool> active{true};
  std::mutex mutex;
  std::condition_variable cv;
  std::queue<std::string> messageQueue;
};

/**
 * SSE Manager (Singleton)
 *
 * Manages all SSE connections and provides methods for broadcasting
 * events to connected clients.
 */
class SSE {
public:
  /**
   * Get the singleton instance.
   */
  static SSE &Instance();

  /**
   * Set up the SSE endpoint on the HTTP server.
   * Call this during route configuration.
   *
   * @param server The httplib server instance
   */
  static void SetupEndpoint(httplib::Server &server);

  /**
   * Broadcast a message to all connected clients.
   *
   * @param eventType The SSE event type (e.g., "selection", "document")
   * @param data JSON data payload
   */
  static void Broadcast(const std::string &eventType, const json &data);

  /**
   * Broadcast a raw message string to all connected clients.
   *
   * @param message Pre-formatted SSE message
   */
  static void BroadcastRaw(const std::string &message);

  /**
   * Send a heartbeat ping to all connected clients.
   * Prevents connection timeout on idle connections.
   */
  static void SendHeartbeat();

  /**
   * Get the number of active connections.
   */
  static size_t ConnectionCount();

  /**
   * Close all active connections.
   */
  static void CloseAll();

private:
  SSE() = default;
  ~SSE() = default;
  SSE(const SSE &) = delete;
  SSE &operator=(const SSE &) = delete;

  // Internal implementation
  void addConnection(std::shared_ptr<SSEConnection> conn);
  void removeConnection(int connId);
  void broadcast(const std::string &message);
  size_t connectionCount();
  void closeAll();

  // Connection management
  std::mutex connectionsMutex_;
  std::vector<std::shared_ptr<SSEConnection>> connections_;
  std::atomic<int> nextConnectionId_{0};
};

/**
 * Event Emitters
 *
 * Convenience functions for emitting common event types.
 * Add domain-specific emitters here as needed.
 */
namespace Events {

/**
 * Emit a selection change event.
 *
 * @param selectedIds Array of selected art handle IDs
 * @param count Number of selected items
 */
void EmitSelectionChange(const std::vector<int> &selectedIds, int count);

/**
 * Emit a document event (open, close, switch).
 *
 * @param eventType The document event type
 * @param documentName Name of the document
 */
void EmitDocumentEvent(const std::string &eventType,
                       const std::string &documentName);

/**
 * Emit a layer change event.
 *
 * @param layerCount Current number of layers
 */
void EmitLayerChange(int layerCount);

/**
 * Emit a generic plugin event.
 *
 * @param eventType Event type string
 * @param data Additional event data
 */
void EmitPluginEvent(const std::string &eventType, const json &data);

/**
 * Emit version info to newly connected clients.
 *
 * @param version Plugin version string
 */
void EmitVersion(const std::string &version);

} // namespace Events

