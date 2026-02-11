# Customizing NUXP for Your Project

NUXP is designed to be forked and customized for your own Adobe Illustrator plugins. This guide walks you through all the places you'll want to update.

## Quick Checklist

- [ ] Plugin name and identity (CMake)
- [ ] Bundle ID (macOS)
- [ ] Package.json fields
- [ ] README branding
- [ ] LICENSE copyright holder

## 1. Plugin Identity (CMake)

Edit `plugin/CMakeLists.txt` or pass options to cmake:

```bash
cmake -B build \
  -DPLUGIN_NAME="MyPlugin" \
  -DPLUGIN_DISPLAY_NAME="My Plugin" \
  -DPLUGIN_VERSION="1.0.0" \
  -DPLUGIN_BUNDLE_ID="com.mycompany.illustrator.myplugin" \
  -DPLUGIN_AUTHOR="Your Name" \
  -DPLUGIN_DESCRIPTION="My awesome Illustrator plugin"
```

Or create a `CMakeUserPresets.json`:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "myplugin",
      "inherits": "default",
      "cacheVariables": {
        "PLUGIN_NAME": "MyPlugin",
        "PLUGIN_DISPLAY_NAME": "My Plugin",
        "PLUGIN_VERSION": "1.0.0",
        "PLUGIN_BUNDLE_ID": "com.mycompany.illustrator.myplugin",
        "PLUGIN_AUTHOR": "Your Name"
      }
    }
  ]
}
```

## 2. Package.json Files

### codegen/package.json

Update these fields:
- `name`: Your package name
- `author`: Your name and email
- `repository`: Your GitHub repository URL
- `homepage`: Your project's homepage
- `bugs`: Your issue tracker URL

### shell/package.json

Update these fields:
- `name`: Your package name
- `description`: Your app description
- `author`: Your name and email
- `repository`: Your GitHub repository URL

## 3. Tauri Configuration

Edit `shell/src-tauri/tauri.conf.json`:

```json
{
  "productName": "My Plugin Shell",
  "identifier": "com.mycompany.myplugin-shell",
  "version": "1.0.0"
}
```

## 4. README.md

Replace the NUXP branding with your own:
- Project name and description
- Logo/badges
- Installation instructions specific to your plugin
- Contact/support information

## 5. LICENSE

Update the copyright holder in `LICENSE`:

```
Copyright (c) 2024 Your Name or Company
```

## 6. GitHub Actions (Optional)

If you're using the CI/CD workflows in `.github/workflows/`:

- Update repository references
- Add your own secrets (e.g., `ILLUSTRATOR_SDK_URL`)
- Customize release configurations

## 7. Plugin Source Code

### Constants in Plugin.hpp

```cpp
// Update these constants
#define NUXP_TIMER_NAME "MyPlugin Timer"
#define NUXP_NOTIFIER_NAME "MyPlugin"
#define NUXP_DEFAULT_PORT 8080
```

### HTTP Server Branding

In `HttpServer.cpp`, update the health/info responses:

```cpp
json response = {
  {"plugin", "My Plugin"},
  {"version", NUXP_VERSION}
};
```

## 8. Frontend Branding

### App.vue / Sidebar.vue

Update the application title and any NUXP references:

```vue
<h1>My Plugin</h1>
```

### Index.html

```html
<title>My Plugin</title>
```

## Files to Search and Replace

For a thorough renaming, search for these terms:
- `NUXP` / `nuxp`
- `Not UXP`
- Default port `8080`
- Bundle ID `com.nuxp`

## Keeping Up with NUXP

If you want to pull in updates from the upstream NUXP repository:

```bash
# Add NUXP as upstream remote
git remote add upstream https://github.com/ORIGINAL/nuxp.git

# Fetch and merge updates
git fetch upstream
git merge upstream/main

# Resolve any conflicts in your customized files
```

## Need Help?

If you encounter issues while customizing NUXP:
1. Check the [GitHub Discussions](https://github.com/ByteBard97/nuxp/discussions)
2. Review existing [Issues](https://github.com/ByteBard97/nuxp/issues)
3. Open a new issue with details about your customization
