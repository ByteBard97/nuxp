# Issue #1 — Build fails without additional headers

## Problem

A fresh checkout of NUXP fails to compile, and even after compilation fixes, the plugin fails to load in Adobe Illustrator 2026 with a "Plugin issues detected" dialog.

## Root causes identified

### RC1: `setup-sdk.sh` crashes on first run (PROVEN)

`trash "$SDK_DEST"` is called unconditionally. If `plugin/sdk/` doesn't exist yet (fresh checkout), the script errors out.

**Fix**: Guard with `if [ -e "$SDK_DEST" ]; then`.

**File**: `scripts/setup-sdk.sh`

### RC2: Missing `#include` directives — build fails (PROVEN)

The generated `IllustratorSDK.h` convenience header omits ~10 SDK headers needed by `SuitePointers.hpp` and the generated endpoint wrappers. Compilation fails with ~20 undeclared type errors (`AIAppContextSuite`, `_t_AIMenuItemOpaque`, `AIBlendStyleSuite`, etc.).

**Fix**: Add missing includes to the `IllustratorSDK.h` heredoc template in `setup-sdk.sh`:

```
AIContext.h, AIUser.h, AIUndo.h, AIMdMemory.h, AIMenu.h,
AIMask.h, AITool.h, AIArtboard.h, AIDictionary.h, AIEntry.h,
IAIArtboards.hpp
```

Note: `AIBlendStyleSuite` is declared inside `AIMask.h` — there is no `AIBlendStyle.h`.

Also add explicit includes to `plugin/src/SuitePointers.hpp` for headers not guaranteed by `IllustratorSDK.h`.

**Files**: `scripts/setup-sdk.sh`, `plugin/src/SuitePointers.hpp`

### RC3: Wrong bundle identity — closed

This is no longer an active hypothesis. The installed working plugins on this
machine (`Phantasm.aip`, `AGCore.aip`) also use `CFBundlePackageType=BNDL` and
`PkgInfo=BNDL????`, matching the current NUXP bundle shape.

### RC4: AINotifier acquisition is fatal — startup loops (HIGH CONFIDENCE, awaiting runtime proof)

In `Plugin.cpp`, if `AcquireSuite(kAINotifierSuite, ...)` fails, `StartupPlugin` returns an error. Illustrator retries startup repeatedly, creating a fatal loop. The previous debugging session confirmed this via runtime logs.

**Fix**: Make the AINotifier suite acquisition non-fatal. Set `sAINotifier = nullptr` on failure and guard all `AddNotifier` calls with `if (sAINotifier)`.

**File**: `plugin/src/Plugin.cpp`

### RC5: HTTP server binds to `localhost` — IPv6-only on modern macOS (CONFIRMED by code analysis)

`HttpServer.cpp` calls `bind_to_port("localhost", port)`. On modern macOS, `localhost` can resolve to `::1` (IPv6 only), making `curl http://127.0.0.1:8080/health` fail with "connection refused" even when the server is running.

**Fix**: Change to `bind_to_port("127.0.0.1", port)`.

**File**: `plugin/src/HttpServer.cpp`

## Current hypothesis ledger

Closed hypotheses:

- Build failures were caused by missing SDK headers.
- `setup-sdk.sh` fresh-checkout crash blocked first-run setup.
- Bundle metadata/signing/deployment target were the only reason Illustrator skipped the plugin.
- Illustrator was only looking in the wrong plugin folder.
- Illustrator prefs, crash-recovery, or plugin cache were the only blocker.
- `ARPI/ART5` bundle identity is required on this machine.

Open hypotheses:

### H1: PiPL `mi32` encoding is invalid for Illustrator 2026

**Prediction**: If `plugin.pipl` is rebuilt with a zero-length `ADBEmi32`
payload, Illustrator will at least `dlopen` the bundle and the constructor log
(`/Users/guillem/Desktop/nuxp-loaded.log`) will appear.

**Evidence**:

- NUXP previously emitted `ADBEmi32` with a 4-byte zero payload.
- Working installed plugins on this machine emit a zero-length `ADBEmi32`
  payload.
- Illustrator sees NUXP's PiPL name but never maps the executable.

**Status**: active

**Test artifact ready**:

- `plugin/tools/pipl/create_pipl.py` patched to emit zero-length `mi32`
- `plugin/CMakeLists.txt` patched so PiPL changes invalidate the build output
- fresh build at `plugin/build-xcode-verify/Release/NUXPPlugin.aip`
- rebuilt `plugin.pipl` is now 96 bytes instead of 100

**Falsifier**: If Illustrator still produces no constructor log after this
bundle is installed into `/Applications/Adobe Illustrator 2026/Plug-ins.localized/`,
close H1 and move on.

### H2: The `.rsrc` file must contain compiled resources, not just an empty fork

**Prediction**: If H1 fails, replacing the synthetic 282-byte `.rsrc` with a
Rez-compiled resource bundle should be the next packaging test.

**Evidence**:

- NUXP currently ships a synthetic 282-byte `.rsrc`.
- Working plugins ship substantial `.rsrc` files.
- The repo's CMake path currently prefers Python PiPL generation and only uses
  Rez as a fallback.

**Status**: active

**Test artifact ready**:

- `plugin/CMakeLists.txt` patched to prefer a Rez-compiled `.rsrc` when Rez is
  available
- local Rez compile succeeds with the existing `plugin/mac/resources/Plugin.r`
- rebuilt bundle at `plugin/build-xcode-verify/Release/NUXPPlugin.aip`
- rebuilt `.rsrc` is now 470 bytes instead of 282

**Falsifier**: If Illustrator still does not `dlopen` the bundle after this
rebuilt plugin is installed, close H2 and move on to the next metadata/entry
structure hypothesis.

## .rsrc file investigation

Our build still generates a 282-byte synthetic resource fork
(`NUXPPlugin.rsrc`). Working plugins have much larger `.rsrc` files (AGCore:
33 KB, Phantasm: 1.3 MB). This is still suspicious, but it is intentionally not
the active hypothesis until the rebuilt PiPL is tested.

## Summary table

| # | Fix | Confidence | Evidence |
|---|-----|-----------|----------|
| 1 | `setup-sdk.sh` guard trash | **Proven** | Script fails without it |
| 2 | Missing SDK headers | **Proven** | Build fails without them |
| 3 | `BNDL/????` bundle identity | **Closed** | Matches working plugins and current NUXP bundle |
| 4 | AINotifier non-fatal | **High** | Previous session confirmed via logs |
| 5 | `127.0.0.1` bind | **High** | Code analysis; `localhost` → IPv6 on macOS |
| 6 | PiPL `mi32` encoding mismatch | **Active** | Known-good plugins use zero-length payload; NUXP used 4-byte payload |
| 7 | Empty synthetic `.rsrc` is insufficient | **Pending** | Working plugins have substantial `.rsrc` payloads |
