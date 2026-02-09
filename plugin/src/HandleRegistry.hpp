#pragma once

#include <cstdint>
#include <shared_mutex>
#include <unordered_map>
#include <mutex>

template <typename T>
class HandleRegistry {
public:
    struct Entry {
        T* ptr;
        uint32_t generation;  // generation when this entry was created
    };

    // Register a pointer, returns a new ID every time.
    // Call this on the main thread (where SDK calls happen).
    int32_t Register(T* ptr) {
        if (!ptr) return 0;

        std::unique_lock<std::shared_mutex> lock(mutex_);
        int32_t id = nextId_++;
        entries_[id] = Entry{ ptr, generation_ };
        return id;
    }

    // Retrieve a pointer by ID. Returns nullptr if:
    //   - ID doesn't exist
    //   - Entry is from a previous generation (stale)
    T* Get(int32_t id) const {
        if (id == 0) return nullptr;

        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = entries_.find(id);
        if (it == entries_.end()) return nullptr;
        if (it->second.generation != generation_) return nullptr;
        return it->second.ptr;
    }

    // Remove a specific pointer (e.g. art was deleted).
    // Removes ALL entries pointing to this address.
    void Invalidate(T* ptr) {
        if (!ptr) return;

        std::unique_lock<std::shared_mutex> lock(mutex_);
        for (auto it = entries_.begin(); it != entries_.end(); ) {
            if (it->second.ptr == ptr) {
                it = entries_.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Bump generation. All existing entries become stale.
    // Call on: document switch, undo, redo, document close.
    void BumpGeneration() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        generation_++;
        entries_.clear();  // no point keeping stale entries
        // Don't reset nextId_ — avoids ID reuse across generations
    }

    // How many live entries (for diagnostics)
    size_t Size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return entries_.size();
    }

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<int32_t, Entry> entries_;
    int32_t nextId_ = 1;      // 0 is reserved for null
    uint32_t generation_ = 0;
};
