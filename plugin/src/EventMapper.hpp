#pragma once

#include "IllustratorSDK.h"
#include "SSE.hpp"
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

/**
 * EventMapper
 *
 * Thread-safe singleton that captures Illustrator events (AINotifierMessage)
 * and queues them as JSON for consumption by the HTTP server via long-polling.
 *
 * The mapper translates raw Illustrator notifier strings (e.g.,
 * "AI Art Selection Changed Notifier") to friendlier event type names
 * (e.g., "selectionChanged") for the frontend.
 */
class EventMapper {
public:
  static EventMapper &Instance() {
    static EventMapper instance;
    return instance;
  }

  /**
   * Add an event to the queue and broadcast via SSE (Thread-safe).
   *
   * @param type The event type - can be a raw Illustrator notifier string
   *             (will be mapped to a friendly name) or a custom event type.
   * @param data Optional JSON data to include with the event.
   */
  void Push(const std::string &type, const json &data = json::object()) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Map the raw notifier string to a friendly name
    std::string friendlyType = MapEventType(type);

    json eventData = {{"type", friendlyType},
                      {"data", data},
                      {"timestamp", std::time(nullptr)}};

    // Store in queue for long-polling clients
    eventQueue_.push_back(eventData);

    // Broadcast via SSE for real-time clients
    SSE::Broadcast(friendlyType, eventData);
  }

  /**
   * Retrieve and clear all pending events (Thread-safe).
   *
   * @return JSON array of all pending events.
   */
  json PopAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    json events = json::array();
    for (const auto &event : eventQueue_) {
      events.push_back(event);
    }
    eventQueue_.clear();
    return events;
  }

  /**
   * Check if queue has pending events (Thread-safe).
   *
   * @return true if there are pending events, false otherwise.
   */
  bool HasEvents() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !eventQueue_.empty();
  }

private:
  EventMapper() { InitializeEventMap(); }
  ~EventMapper() = default;

  // Disable copy/move
  EventMapper(const EventMapper &) = delete;
  EventMapper &operator=(const EventMapper &) = delete;

  /**
   * Initialize the mapping from raw Illustrator notifier strings
   * to friendly event type names.
   */
  void InitializeEventMap() {
    // Art/Selection events
    eventTypeMap_[kAIArtSelectionChangedNotifier] = "selectionChanged";
    eventTypeMap_[kAIArtPropertiesChangedNotifier] = "artPropertiesChanged";
    eventTypeMap_[kAIArtObjectsChangedNotifier] = "artChanged";
    eventTypeMap_[kAIDrawingModeChangedNotifier] = "drawingModeChanged";

    // Document events
    eventTypeMap_[kAIDocumentChangedNotifier] = "documentChanged";
    eventTypeMap_[kAIDocumentClosedNotifier] = "documentClosed";
    eventTypeMap_[kAIDocumentOpenedNotifier] = "documentOpened";
    eventTypeMap_[kAIDocumentNewNotifier] = "documentNew";
    eventTypeMap_[kAIDocumentSavedNotifier] = "documentSaved";

    // Layer events
    eventTypeMap_[kAILayerListChangedNotifier] = "layersChanged";
  }

  /**
   * Map a raw Illustrator notifier string to a friendly event type name.
   *
   * @param rawType The raw notifier string from AINotifierMessage::type
   * @return The friendly event type name, or the raw type if no mapping exists.
   */
  std::string MapEventType(const std::string &rawType) const {
    auto it = eventTypeMap_.find(rawType);
    if (it != eventTypeMap_.end()) {
      return it->second;
    }
    // If no mapping exists, return the raw type
    // This allows custom event types to pass through unchanged
    return rawType;
  }

  mutable std::mutex mutex_;
  std::vector<json> eventQueue_;
  std::unordered_map<std::string, std::string> eventTypeMap_;
};
