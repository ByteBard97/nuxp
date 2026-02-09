# Third-party Libraries

This directory contains header-only C++ libraries used by the plugin.

## Included

*   `httplib.h` - [cpp-httplib](https://github.com/yhirose/cpp-httplib) v0.14+
    - HTTP server library for the JSON API
    - Header-only, requires C++11

*   `nlohmann/json.hpp` - [nlohmann/json](https://github.com/nlohmann/json) v3.11.3
    - JSON parsing and serialization library
    - Header-only, requires C++11
    - Used for HTTP request/response handling

## Usage

Include in C++ files using:
```cpp
#include <httplib.h>
#include <nlohmann/json.hpp>
```

The CMakeLists.txt automatically adds this directory to the include path.
