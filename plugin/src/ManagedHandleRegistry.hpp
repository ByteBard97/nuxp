#pragma once

#include <cstdint>
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <mutex>

// Owning registry: stores objects via unique_ptr<T>.
// Use this for C++ objects whose lifetime the plugin controls
// (e.g. AIColor, AIPathStyle), as opposed to HandleRegistry which
// stores non-owning raw pointers to SDK-owned handles.
template <typename T>
class ManagedHandleRegistry {
public:
    struct Entry {
        std::unique_ptr<T> obj;
        uint32_t generation;  // generation when this entry was created
    };

    // Register by moving an object in. Returns a new ID.
    int32_t Register(T&& obj) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        int32_t id = nextId_++;
        entries_[id] = Entry{ std::make_unique<T>(std::move(obj)), generation_ };
        return id;
    }

    // Register by copying an object in. Returns a new ID.
    int32_t Register(const T& obj) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        int32_t id = nextId_++;
        entries_[id] = Entry{ std::make_unique<T>(obj), generation_ };
        return id;
    }

    // Retrieve a raw pointer to the owned object by ID.
    // Returns nullptr if:
    //   - ID is 0 or doesn't exist
    //   - Entry is from a previous generation (stale)
    T* Get(int32_t id) const {
        if (id == 0) return nullptr;

        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = entries_.find(id);
        if (it == entries_.end()) return nullptr;
        if (it->second.generation != generation_) return nullptr;
        return it->second.obj.get();
    }

    // Explicitly destroy and remove an entry by ID.
    void Remove(int32_t id) {
        if (id == 0) return;

        std::unique_lock<std::shared_mutex> lock(mutex_);
        entries_.erase(id);
    }

    // Bump generation. All existing entries become stale and are destroyed.
    // Call on: document switch, undo, redo, document close.
    void BumpGeneration() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        generation_++;
        entries_.clear();  // unique_ptrs auto-destruct
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
