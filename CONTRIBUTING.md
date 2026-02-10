# Contributing to NUXP

Thank you for your interest in contributing to NUXP! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites

- **macOS or Windows** (required for Adobe Illustrator)
- **Adobe Illustrator 2024+** (SDK version must match)
- **Adobe Illustrator SDK** - Download from [Adobe Developer Console](https://developer.adobe.com)
- **CMake 3.20+**
- **Node.js 18+**
- **C++17 compatible compiler** (Xcode on macOS, MSVC on Windows)

### Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/nuxp.git
   cd nuxp
   ```

2. **Set up the Adobe SDK**
   ```bash
   # Download the SDK DMG from Adobe Developer Console
   ./scripts/setup-sdk.sh ~/Downloads/AI_2024_SDK_Mac.dmg
   ```

3. **Build the C++ plugin**
   ```bash
   cd plugin
   cmake -B build
   cmake --build build
   cmake --install build  # Installs to Illustrator Plug-ins folder
   ```

4. **Set up the frontend**
   ```bash
   cd shell
   npm install
   npm run dev  # Starts dev server with hot reload
   ```

5. **Set up the code generator**
   ```bash
   cd codegen
   npm install
   npm test  # Verify tests pass
   ```

## Project Structure

```
nuxp/
├── plugin/           # C++ Illustrator plugin
│   ├── src/          # Core plugin code
│   ├── src/endpoints/        # Hand-written endpoints
│   └── src/endpoints/generated/  # Auto-generated (DO NOT EDIT)
├── shell/            # Vue 3 frontend
│   └── src/sdk/      # TypeScript bridge layer
├── codegen/          # SDK parser + code generators
└── scripts/          # Build automation
```

## Making Changes

### Branching Strategy

- Create feature branches from `main`
- Use descriptive branch names: `feature/add-gradient-support`, `fix/handle-leak`

### Code Style

**C++ (plugin/)**
- Follow existing formatting (clang-format configuration coming soon)
- Use modern C++17 features where appropriate
- Document non-obvious code with comments
- Handle errors gracefully - never crash the plugin

**TypeScript (shell/, codegen/)**
- Run `npm run type-check` before committing
- Follow existing patterns for new components/modules

### Adding New Endpoints

#### Hand-written Endpoints

For custom functionality not covered by SDK wrappers:

1. Create `plugin/src/endpoints/YourEndpoints.cpp` and `.hpp`
2. Define handler functions that accept JSON and return JSON
3. Register routes in `HttpServer::ConfigureRoutes()`
4. Add TypeScript client in `shell/src/sdk/`

#### Generated Endpoints

To wrap additional SDK suites:

1. Identify the SDK header file (e.g., `AIGradient.h`)
2. Add the header to `codegen/src/config/suites.json`
3. Run `./scripts/generate.sh`
4. Rebuild the plugin

### Testing

**Codegen tests**
```bash
cd codegen && npm test
```

**Frontend type checking**
```bash
cd shell && npm run type-check
```

**Plugin testing**
- Build and install the plugin
- Launch Illustrator
- Use the Debug Panel or curl to test endpoints

## Pull Request Guidelines

1. **Create focused PRs** - One feature or fix per PR
2. **Write descriptive titles** - "Add gradient fill support" not "Update code"
3. **Include context** - Explain what changes and why
4. **Test your changes** - Verify in Illustrator when possible
5. **Update documentation** - If adding features, update relevant docs

### PR Template

When creating a PR, please include:

- **Summary**: What does this PR do?
- **Testing**: How did you verify this works?
- **Breaking changes**: Any API or behavior changes?

## Commit Guidelines

- Write clear, concise commit messages
- Use present tense: "Add feature" not "Added feature"
- Reference issues when applicable: "Fix #123: Handle stale references"

## Getting Help

- **Issues**: Report bugs or request features via GitHub Issues
- **Discussions**: Ask questions in GitHub Discussions

## License

By contributing to NUXP, you agree that your contributions will be licensed under the MIT License.
