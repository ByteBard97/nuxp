//========================================================================================
//
//  NuxpThreadSafety.h
//
//  Compile-time safety annotations for Adobe Illustrator SDK calls.
//
//  Covers three classes of bugs that have caused real crashes:
//
//  1. THREAD SAFETY — SDK calls from non-main thread crash Illustrator.
//     Annotate SDK functions with REQUIRES_MAIN_THREAD.
//     Entry points called by Illustrator on the main thread get GRANTS_MAIN_THREAD.
//     Enable checker: -Wthread-safety
//
//  2. IGNORED ERRORS — ASErr / bool return values silently dropped → corrupted state.
//     Annotate error-returning functions with NODISCARD_ERR.
//     Callers that ignore the return value get a compile-time warning.
//     Enable checker: -Wunused-result (included in -Wall)
//
//  3. RUNTIME DEFENSE — ASSERT_MAIN_THREAD() fires in debug builds if an SDK
//     function is somehow reached from the wrong thread at runtime.
//
//  Quick reference
//  ---------------
//  REQUIRES_MAIN_THREAD   — function must be called on the main thread
//  GRANTS_MAIN_THREAD     — function runs on the main thread (entry point)
//  RELEASES_MAIN_THREAD   — function exits the main-thread context
//  NO_THREAD_SAFETY       — intentionally skip thread-safety check (document why)
//  NODISCARD_ERR          — caller must not ignore the error return value
//  ASSERT_MAIN_THREAD()   — runtime assertion (debug builds only, zero cost in release)
//
//  Compiler support
//  ----------------
//  Requires Apple Clang (or Clang 3.6+). All macros compile away to nothing on
//  other compilers so Windows builds are unaffected.
//
//  Downstream use
//  --------------
//  FloraBridge includes this header via FloraThreadSafety.h.
//  gMainThread and gMainThreadId are defined in each plugin's entry point
//  (Plugin.cpp for standalone NUXP, FloraBridgePlugin.cpp for FloraBridge).
//
//========================================================================================

#ifndef NUXP_THREAD_SAFETY_H
#define NUXP_THREAD_SAFETY_H

#include <cassert>
#include <thread>

// -------------------------------------------------------------------------
// Annotation macros — active only when Clang thread safety analysis is on.
// All other compilers see empty macros so existing code compiles unchanged.
// -------------------------------------------------------------------------

#if defined(__clang__)
  #define NUXP_CAPABILITY(x)          __attribute__((capability(x)))
  #define NUXP_SCOPED_CAPABILITY      __attribute__((scoped_lockable))
  #define NUXP_REQUIRES(...)          __attribute__((requires_capability(__VA_ARGS__)))
  #define NUXP_ACQUIRE(...)           __attribute__((acquire_capability(__VA_ARGS__)))
  #define NUXP_RELEASE(...)           __attribute__((release_capability(__VA_ARGS__)))
  #define NUXP_NO_THREAD_SAFETY       __attribute__((no_thread_safety_analysis))
  #define NUXP_NODISCARD              __attribute__((warn_unused_result))
#else
  // Non-Clang compiler (MSVC, GCC) — strip all annotations
  #define NUXP_CAPABILITY(x)
  #define NUXP_SCOPED_CAPABILITY
  #define NUXP_REQUIRES(...)
  #define NUXP_ACQUIRE(...)
  #define NUXP_RELEASE(...)
  #define NUXP_NO_THREAD_SAFETY
  #define NUXP_NODISCARD
#endif

// Convenience aliases matching the Adobe SDK convention in this codebase
#define REQUIRES_MAIN_THREAD   NUXP_REQUIRES(gMainThread)
#define GRANTS_MAIN_THREAD     NUXP_ACQUIRE(gMainThread)
#define RELEASES_MAIN_THREAD   NUXP_RELEASE(gMainThread)
#define NO_THREAD_SAFETY       NUXP_NO_THREAD_SAFETY

// Mark an error-returning function so callers CANNOT silently drop the result.
// Usage: ASErr MyFunc() NODISCARD_ERR;
#define NODISCARD_ERR          NUXP_NODISCARD

// -------------------------------------------------------------------------
// MainThreadCapability — the "lock" that models Illustrator's main thread.
//
// Declare gMainThread as a global capability. SDK-calling functions declare
// REQUIRES_MAIN_THREAD; entry points from Illustrator declare GRANTS_MAIN_THREAD.
// The compiler then verifies that every REQUIRES path has a GRANTS ancestor.
// -------------------------------------------------------------------------

class NUXP_CAPABILITY("main thread") MainThreadCapability {
public:
    // These methods are never called at runtime — they exist only so the
    // compiler can track acquire/release edges in the capability graph.
    // NUXP_NO_THREAD_SAFETY on the methods themselves prevents the analyzer
    // from recursively checking their own acquire/release annotations.
    void acquire() NUXP_ACQUIRE(this) NUXP_NO_THREAD_SAFETY {}
    void release() NUXP_RELEASE(this) NUXP_NO_THREAD_SAFETY {}
};

// The single global capability object.
// Defined in each plugin's entry point (Plugin.cpp / FloraBridgePlugin.cpp).
extern MainThreadCapability gMainThread;

// The thread ID of Illustrator's main thread, set during plugin startup.
// Used by ASSERT_MAIN_THREAD() for defense-in-depth runtime checking.
extern std::thread::id gMainThreadId;

// -------------------------------------------------------------------------
// ASSERT_MAIN_THREAD() — runtime defense-in-depth.
//
// Fires an assertion in debug builds if called from a non-main thread.
// Place at the top of any function that calls into the Illustrator SDK.
// Does NOT replace the static annotations — use both.
// -------------------------------------------------------------------------

#ifdef NDEBUG
  // Release builds: no overhead
  #define ASSERT_MAIN_THREAD() ((void)0)
#else
  #define ASSERT_MAIN_THREAD()                                                \
    do {                                                                      \
      assert(std::this_thread::get_id() == gMainThreadId &&                  \
             "SDK call from non-main thread — will crash Illustrator!");      \
    } while (false)
#endif

#endif // NUXP_THREAD_SAFETY_H
