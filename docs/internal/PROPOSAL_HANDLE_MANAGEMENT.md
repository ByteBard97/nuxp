# Handle Management v2: Typed Registries with Generation Safety

**Status**: Proposed revision of `PROPOSAL_HANDLE_MANAGEMENT.md`  
**Context**: NUXP (Tauri + Adobe Illustrator C++ SDK)

---

## Changes from v1

1. **Typed registries** instead of `void*` — compile-time prevention of handle type misuse
2. **Generation counter** — detects stale handles after undo/redo/document switch
3. **Thread safety** — `std::shared_mutex` for concurrent HTTP reads + exclusive SDK writes
4. **No reverse map** — avoids pointer-reuse bugs; always assigns fresh IDs
5. **Scoped lifecycle** — registry clears on document switch and undo/redo

---

## Core: `HandleRegistry<T>`

A single-type registry. One instance per handle type.

```cpp
// HandleRegistry.hpp
#pragma once

#include <cstdint>
#include <shared_mutex>
#include <unordered_map>

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

        std::unique_lock lock(mutex_);
        int32_t id = nextId_++;
        entries_[id] = Entry{ ptr, generation_ };
        return id;
    }

    // Retrieve a pointer by ID. Returns nullptr if:
    //   - ID doesn't exist
    //   - Entry is from a previous generation (stale)
    T* Get(int32_t id) const {
        if (id == 0) return nullptr;

        std::shared_lock lock(mutex_);
        auto it = entries_.find(id);
        if (it == entries_.end()) return nullptr;
        if (it->second.generation != generation_) return nullptr;
        return it->second.ptr;
    }

    // Remove a specific pointer (e.g. art was deleted).
    // Removes ALL entries pointing to this address.
    void Invalidate(T* ptr) {
        if (!ptr) return;

        std::unique_lock lock(mutex_);
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
        std::unique_lock lock(mutex_);
        generation_++;
        entries_.clear();  // no point keeping stale entries
        // Don't reset nextId_ — avoids ID reuse across generations
    }

    // How many live entries (for diagnostics)
    size_t Size() const {
        std::shared_lock lock(mutex_);
        return entries_.size();
    }

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<int32_t, Entry> entries_;
    int32_t nextId_ = 1;      // 0 is reserved for null
    uint32_t generation_ = 0;
};
```

---

## Global Registries: `HandleManager`

One registry per SDK handle type. Thin static wrapper.

```cpp
// HandleManager.hpp
#pragma once

#include "HandleRegistry.hpp"
#include "IllustratorSDK.h"

class HandleManager {
public:
    // --- Per-type registries ---
    static HandleRegistry<AIArtHandle>       art;       // cast note below
    static HandleRegistry<AILayerHandle>     layers;
    static HandleRegistry<AIDocumentHandle>  documents;
    // Add more as needed:
    // static HandleRegistry<AIPatternHandle>   patterns;
    // static HandleRegistry<AISymbolHandle>    symbols;

    // Bump all registries at once (document switch, undo/redo)
    static void InvalidateAll() {
        art.BumpGeneration();
        layers.BumpGeneration();
        documents.BumpGeneration();
    }
};

// HandleManager.cpp
HandleRegistry<AIArtHandle>       HandleManager::art;
HandleRegistry<AILayerHandle>     HandleManager::layers;
HandleRegistry<AIDocumentHandle>  HandleManager::documents;
```

> **Note on AIArtHandle / AILayerHandle**: These are typically `typedef struct _opaque* AIArtHandle;` — pointer-to-opaque-struct. The registry stores `T*` which means `AIArtHandle*` — a pointer to a pointer. If your SDK typedefs are already pointers, you may need `HandleRegistry<std::remove_pointer_t<AIArtHandle>>` or simply specialize for the raw typedef. Adjust based on your SDK's actual type definitions. The important thing is that `art.Register(myArtHandle)` and `art.Get(id)` round-trip correctly.

---

## Notifier Integration

Hook these into your existing notifier handlers:

```cpp
// In your plugin's Notify() handler:

void FloraPlugin::Notify(AINotifierMessage* message) {
    
    // Document switched — all handles from previous doc are garbage
    if (message->notifier == fDocumentChangedNotifier) {
        HandleManager::InvalidateAll();
    }
    
    // Undo/Redo — art handles may point to freed or different objects
    if (message->notifier == fUndoNotifier || 
        message->notifier == fRedoNotifier) {
        HandleManager::InvalidateAll();
    }
    
    // Art deleted — surgically remove just that handle
    // This covers user-initiated deletes between generation bumps
    if (message->notifier == fArtObjectsChangedNotifier) {
        ArtObjectsChangedNotifierData* data = 
            static_cast<ArtObjectsChangedNotifierData*>(message->d.pointer);
        
        if (data) {
            // Iterate removed UUIDs list — resolve to AIArtHandle and invalidate
            // (Implementation depends on how you track art<->handle mapping)
            // For bulk changes, just bump generation:
            if (data->removedCount > 10) {
                HandleManager::art.BumpGeneration();
            }
        }
    }
    
    // Document closing
    if (message->notifier == fDocumentClosedNotifier) {
        HandleManager::InvalidateAll();
    }
}
```

---

## Generated Endpoint Example

Here's what the code generator would produce for `AILayerSuite::CountLayers`:

```cpp
// No handles involved — pure value return
void RegisterLayerEndpoints(httplib::Server& server) {
    
    server.Get("/layers/count", [](const httplib::Request&, httplib::Response& res) {
        // Marshal to main thread via work queue
        auto result = RunOnMainThread([]() -> json {
            ai::int32 count = 0;
            AIErr err = sAILayer->CountLayers(&count);
            if (err) return json{{ "error", AIErrToString(err) }};
            return json{{ "count", count }};
        });
        
        res.set_content(result.dump(), "application/json");
    });
}
```

And for `AILayerSuite::GetNthLayer` (returns a handle):

```cpp
server.Get("/layers/:index", [](const httplib::Request& req, httplib::Response& res) {
    int n = std::stoi(req.path_params.at("index"));
    
    auto result = RunOnMainThread([n]() -> json {
        AILayerHandle layer = nullptr;
        AIErr err = sAILayer->GetNthLayer(n, &layer);
        if (err) return json{{ "error", AIErrToString(err) }};
        
        // Register handle, get ID for frontend
        int32_t layerId = HandleManager::layers.Register(layer);
        
        // Also fetch useful properties while we're on the main thread
        ai::UnicodeString name;
        sAILayer->GetLayerTitle(layer, name);
        
        AIBoolean visible, editable;
        sAILayer->GetLayerVisible(layer, &visible);
        sAILayer->GetLayerEditable(layer, &editable);
        
        return json{
            { "handle",   layerId },
            { "name",     name.as_UTF8() },
            { "visible",  (bool)visible },
            { "editable", (bool)editable }
        };
    });
    
    res.set_content(result.dump(), "application/json");
});
```

And for an endpoint that *receives* a handle from the frontend:

```cpp
server.Post("/layers/set_visible", [](const httplib::Request& req, httplib::Response& res) {
    auto body = json::parse(req.body);
    int32_t layerId = body["handle"];
    bool visible = body["visible"];
    
    auto result = RunOnMainThread([layerId, visible]() -> json {
        // Look up the real handle
        AILayerHandle layer = HandleManager::layers.Get(layerId);
        if (!layer) {
            return json{{ "error", "stale_handle" }, { "message", 
                "Layer handle is no longer valid. Re-fetch layers." }};
        }
        
        AIErr err = sAILayer->SetLayerVisible(layer, visible);
        if (err) return json{{ "error", AIErrToString(err) }};
        return json{{ "ok", true }};
    });
    
    res.set_content(result.dump(), "application/json");
});
```

---

## `RunOnMainThread` Helper

This is the timer-queue pattern from our earlier discussion, wrapped in a clean API:

```cpp
// MainThreadDispatch.hpp
#pragma once

#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include "IllustratorSDK.h"

using WorkItem = std::packaged_task<json()>;

class MainThreadDispatch {
public:
    // Called from HTTP thread — blocks until main thread executes the work
    template <typename F>
    static auto Run(F&& fn) -> decltype(fn()) {
        using ReturnType = decltype(fn());
        
        auto task = std::packaged_task<ReturnType()>(std::forward<F>(fn));
        auto future = task.get_future();
        
        {
            std::lock_guard lock(queueMutex_);
            workQueue_.push(std::move(task));
        }
        
        // Block until main thread picks it up
        return future.get();
    }
    
    // Called from AITimerSuite callback (main thread, every ~16ms)
    static void ProcessQueue() {
        std::lock_guard lock(queueMutex_);
        while (!workQueue_.empty()) {
            auto task = std::move(workQueue_.front());
            workQueue_.pop();
            task();  // executes on main thread
        }
    }

private:
    static std::mutex queueMutex_;
    static std::queue<std::packaged_task<json()>> workQueue_;
};

// Convenience macro for generated code
#define RunOnMainThread(fn) MainThreadDispatch::Run(fn)
```

> **Note**: The `std::packaged_task` in the queue is typed to `json()`. If you need endpoints that return other types, either always serialize to json before returning, or use `std::function<void()>` with a captured promise. The json approach is simpler since every HTTP endpoint returns JSON anyway.

---

## Frontend TypeScript Client

The generated TypeScript would look like this:

```typescript
// nuxp-client/src/sdk/layers.ts

const BASE = "http://localhost:8081";

export interface LayerInfo {
    handle: number;    // opaque ID — don't inspect, just pass back
    name: string;
    visible: boolean;
    editable: boolean;
}

export interface SDKError {
    error: string;
    message?: string;
}

function isError(res: any): res is SDKError {
    return res.error !== undefined;
}

export async function getLayerCount(): Promise<number> {
    const res = await fetch(`${BASE}/layers/count`);
    const data = await res.json();
    if (isError(data)) throw new Error(data.error);
    return data.count;
}

export async function getLayer(index: number): Promise<LayerInfo> {
    const res = await fetch(`${BASE}/layers/${index}`);
    const data = await res.json();
    if (isError(data)) throw new Error(data.error);
    return data;
}

export async function setLayerVisible(handle: number, visible: boolean): Promise<void> {
    const res = await fetch(`${BASE}/layers/set_visible`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ handle, visible }),
    });
    const data = await res.json();
    if (isError(data)) {
        if (data.error === "stale_handle") {
            // Handle went stale (undo/redo/doc switch happened)
            // Caller should re-fetch layer list
            throw new StaleHandleError(data.message);
        }
        throw new Error(data.error);
    }
}

// Custom error type so callers can catch and re-fetch
export class StaleHandleError extends Error {
    constructor(message?: string) {
        super(message ?? "Handle is no longer valid. Re-fetch from SDK.");
        this.name = "StaleHandleError";
    }
}
```

---

## Usage in Vue Component

```vue
<script setup lang="ts">
import { ref, onMounted } from "vue";
import { getLayerCount, getLayer, setLayerVisible, StaleHandleError } from "@/sdk/layers";
import type { LayerInfo } from "@/sdk/layers";

const layers = ref<LayerInfo[]>([]);

async function fetchLayers() {
    const count = await getLayerCount();
    const results: LayerInfo[] = [];
    for (let i = 0; i < count; i++) {
        results.push(await getLayer(i));
    }
    layers.value = results;
}

async function toggleVisibility(layer: LayerInfo) {
    try {
        await setLayerVisible(layer.handle, !layer.visible);
        layer.visible = !layer.visible;
    } catch (e) {
        if (e instanceof StaleHandleError) {
            // Handles went stale — re-fetch everything
            await fetchLayers();
        } else {
            throw e;
        }
    }
}

onMounted(fetchLayers);
</script>
```

---

## Design Decisions

### Why no reverse map?

v1 had `ptrToId` to deduplicate — if the same pointer was registered twice, it returned the same ID. The problem: if Illustrator frees object A at `0x1234` and allocates object B at `0x1234`, the reverse map returns A's ID. The frontend still thinks that ID refers to A.

By always assigning fresh IDs, the worst case is wasted map entries (which get cleared on generation bump anyway). The upside is correctness.

### Why generation counter instead of per-entry invalidation?

Undo/redo can invalidate dozens or hundreds of handles at once, and we may not get individual notifications for each one. Bumping the generation is O(1) and guarantees safety. The registry clears stale entries on bump rather than letting them accumulate.

### Why clear entries on generation bump?

If we just bumped the counter but kept entries, the map would grow without bound across long editing sessions. Since stale entries can never be used (generation check fails), there's no reason to keep them.

### Why `std::shared_mutex`?

HTTP GET endpoints (which call `Get()`) can run concurrently — they only need a read lock. `Register()` and `BumpGeneration()` need exclusive access. `shared_mutex` allows multiple concurrent readers with exclusive writers. For this workload (many reads, few writes), it's the right choice.

### What about batch endpoints?

For endpoints like `GET /layers` that return multiple items, register all handles in a single main-thread call to avoid round-tripping:

```cpp
server.Get("/layers", [](const httplib::Request&, httplib::Response& res) {
    auto result = RunOnMainThread([]() -> json {
        ai::int32 count = 0;
        sAILayer->CountLayers(&count);
        
        json layers = json::array();
        for (int i = 0; i < count; i++) {
            AILayerHandle layer = nullptr;
            sAILayer->GetNthLayer(i, &layer);
            
            ai::UnicodeString name;
            sAILayer->GetLayerTitle(layer, name);
            
            AIBoolean visible, editable;
            sAILayer->GetLayerVisible(layer, &visible);
            sAILayer->GetLayerEditable(layer, &editable);
            
            layers.push_back({
                { "handle",   HandleManager::layers.Register(layer) },
                { "name",     name.as_UTF8() },
                { "visible",  (bool)visible },
                { "editable", (bool)editable },
                { "index",    i }
            });
        }
        
        return json{{ "layers", layers }};
    });
    
    res.set_content(result.dump(), "application/json");
});
```
