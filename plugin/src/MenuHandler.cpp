/**
 * MenuHandler - Plugin menu implementation
 */

#include "MenuHandler.hpp"
#include "ConfigManager.hpp"
#include "HttpServer.hpp"
#include "AIMenuGroups.h"

#include <string>

// Platform-specific dialog includes
#ifdef MAC_ENV
#include <CoreFoundation/CoreFoundation.h>
#endif

// Menu suite pointer
static AIMenuSuite* sAIMenu = nullptr;

// Menu item handle
static AIMenuItemHandle gConfigMenuItem = nullptr;

// Forward declarations
extern "C" SPBasicSuite* sSPBasic;

namespace MenuHandler {

ASErr Initialize(SPPluginRef pluginRef) {
    ASErr error = kNoErr;

    // Acquire menu suite
    if (sSPBasic != nullptr) {
        const void* suite = nullptr;
        error = sSPBasic->AcquireSuite(kAIMenuSuite, kAIMenuSuiteVersion, &suite);
        if (error == kNoErr && suite != nullptr) {
            sAIMenu = const_cast<AIMenuSuite*>(static_cast<const AIMenuSuite*>(suite));
        }
    }

    if (sAIMenu == nullptr) {
        return kCantHappenErr;
    }

    // Add menu item to Help menu
    // Using AddMenuItemZString for simpler API
    AIPlatformAddMenuItemDataUS menuData;
    menuData.groupName = kHelpMenuGroup;
    menuData.itemText = ai::UnicodeString("NUXP Server Settings...");

    error = sAIMenu->AddMenuItem(
        pluginRef,
        "NUXP_ConfigPort",  // Keyboard shortcut dictionary key
        &menuData,
        0,  // No special options
        &gConfigMenuItem
    );

    return error;
}

ASErr HandleMenu(AIMenuMessage* message) {
    if (message == nullptr || message->menuItem == nullptr) {
        return kNoErr;
    }

    // Check if it's our menu item
    if (message->menuItem == gConfigMenuItem) {
        ShowPortConfigDialog();
    }

    return kNoErr;
}

void Shutdown() {
    // Release menu suite
    if (sSPBasic != nullptr && sAIMenu != nullptr) {
        sSPBasic->ReleaseSuite(kAIMenuSuite, kAIMenuSuiteVersion);
        sAIMenu = nullptr;
    }
    gConfigMenuItem = nullptr;
}

#ifdef MAC_ENV
// macOS implementation using Core Foundation
bool ShowPortConfigDialog() {
    int currentPort = ConfigManager::Instance().GetPort();

    // Create alert
    CFStringRef title = CFSTR("NUXP Server Settings");
    CFStringRef message = CFStringCreateWithFormat(
        nullptr, nullptr,
        CFSTR("Enter the HTTP server port (current: %d)\n\nValid range: %d - %d\nDefault: 8080"),
        currentPort,
        ConfigManager::MIN_PORT,
        ConfigManager::MAX_PORT
    );

    // Use CFUserNotification for a simple input dialog
    CFStringRef defaultValue = CFStringCreateWithFormat(nullptr, nullptr, CFSTR("%d"), currentPort);

    CFOptionFlags responseFlags;
    CFStringRef responseValue = nullptr;

    // Keys for the dialog
    const void* keys[] = {
        kCFUserNotificationAlertHeaderKey,
        kCFUserNotificationAlertMessageKey,
        kCFUserNotificationTextFieldTitlesKey,
        kCFUserNotificationTextFieldValuesKey,
        kCFUserNotificationDefaultButtonTitleKey,
        kCFUserNotificationAlternateButtonTitleKey
    };

    CFStringRef fieldTitle = CFSTR("Port:");
    CFArrayRef fieldTitles = CFArrayCreate(nullptr, (const void**)&fieldTitle, 1, &kCFTypeArrayCallBacks);
    CFArrayRef fieldValues = CFArrayCreate(nullptr, (const void**)&defaultValue, 1, &kCFTypeArrayCallBacks);

    const void* values[] = {
        title,
        message,
        fieldTitles,
        fieldValues,
        CFSTR("OK"),
        CFSTR("Cancel")
    };

    CFDictionaryRef dict = CFDictionaryCreate(
        nullptr,
        keys, values,
        sizeof(keys) / sizeof(keys[0]),
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );

    SInt32 dialogError = 0;
    CFUserNotificationRef notification = CFUserNotificationCreate(
        nullptr,
        0,  // No timeout
        kCFUserNotificationPlainAlertLevel,
        &dialogError,
        dict
    );

    CFRelease(dict);
    CFRelease(fieldTitles);
    CFRelease(fieldValues);
    CFRelease(message);
    CFRelease(defaultValue);

    if (dialogError != 0 || notification == nullptr) {
        if (notification != nullptr) {
            CFRelease(notification);
        }
        return false;
    }

    // Wait for response
    CFUserNotificationReceiveResponse(notification, 0, &responseFlags);

    bool result = false;

    // Check if OK was pressed (button 0)
    if ((responseFlags & 0x3) == kCFUserNotificationDefaultResponse) {
        // Get the entered value
        responseValue = CFUserNotificationGetResponseValue(notification, kCFUserNotificationTextFieldValuesKey, 0);

        if (responseValue != nullptr) {
            char buffer[32];
            if (CFStringGetCString(responseValue, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
                int newPort = atoi(buffer);

                if (newPort >= ConfigManager::MIN_PORT && newPort <= ConfigManager::MAX_PORT) {
                    if (newPort != currentPort) {
                        // Save new port
                        ConfigManager::Instance().SetPort(newPort);
                        ConfigManager::Instance().Save();

                        // Restart HTTP server
                        HttpServer::Stop();
                        HttpServer::Start(newPort);

                        result = true;
                    }
                }
            }
        }
    }

    CFRelease(notification);
    return result;
}

#elif defined(WIN_ENV)
// Windows implementation
#include <windows.h>

bool ShowPortConfigDialog() {
    int currentPort = ConfigManager::Instance().GetPort();

    // Simple MessageBox-based approach
    // For a real implementation, you'd create a proper dialog resource
    char message[256];
    snprintf(message, sizeof(message),
        "Current port: %d\n\n"
        "To change the port, edit the config file:\n"
        "%%APPDATA%%\\NUXP\\config.json\n\n"
        "Valid range: %d - %d",
        currentPort,
        ConfigManager::MIN_PORT,
        ConfigManager::MAX_PORT
    );

    MessageBoxA(nullptr, message, "NUXP Server Settings", MB_OK | MB_ICONINFORMATION);

    return false;
}

#else
// Fallback for other platforms
bool ShowPortConfigDialog() {
    return false;
}
#endif

} // namespace MenuHandler
