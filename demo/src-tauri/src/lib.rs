// NUXP Shell - Tauri Application
// Provides standalone desktop app functionality with asset resolution,
// directory listing, and a Unix socket CLI bridge for debug JS execution.

use std::path::PathBuf;
use tauri::{Manager, Listener};

/// Error type for asset path resolution
#[derive(Debug, thiserror::Error)]
pub enum AssetError {
    #[error("Resource directory not found")]
    ResourceDirNotFound,
    #[error("Asset not found: {0}")]
    AssetNotFound(String),
    #[error("Invalid path: {0}")]
    InvalidPath(String),
    #[error("Path conversion error")]
    PathConversionError,
}

// Implement serialization for the error type so it can be returned from commands
impl serde::Serialize for AssetError {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        serializer.serialize_str(&self.to_string())
    }
}

/// Resolves a relative asset path to an absolute filesystem path.
///
/// Used by the frontend to get absolute paths for files that need to be
/// referenced by native code or external processes.
///
/// # Arguments
/// * `relative_path` - A relative path to an asset (e.g., "symbols/tree.svg")
/// * `app_handle` - Tauri app handle for accessing the resource directory
///
/// # Returns
/// * `Ok(String)` - The absolute filesystem path to the asset
/// * `Err(AssetError)` - Error if the asset cannot be resolved
#[tauri::command]
fn get_asset_path(relative_path: String, app_handle: tauri::AppHandle) -> Result<String, AssetError> {
    // Validate the relative path - prevent path traversal attacks
    if relative_path.contains("..") {
        return Err(AssetError::InvalidPath(
            "Path traversal not allowed".to_string(),
        ));
    }

    // Normalize path separators for cross-platform compatibility
    let normalized_path = relative_path.replace('\\', "/");

    // Get the resource directory from Tauri
    // In development, this is typically the src-tauri directory
    // In production, this is the bundled resources directory
    let resource_dir = app_handle
        .path()
        .resource_dir()
        .map_err(|_| AssetError::ResourceDirNotFound)?;

    // Construct the full path to the asset
    // Assets are bundled under the "assets" subdirectory in resources
    let asset_path: PathBuf = resource_dir.join("assets").join(&normalized_path);

    // Verify the file exists
    if !asset_path.exists() {
        // Try alternative: maybe the relative_path already includes "assets/"
        let alt_path: PathBuf = resource_dir.join(&normalized_path);
        if alt_path.exists() {
            return alt_path
                .to_str()
                .map(|s| s.to_string())
                .ok_or(AssetError::PathConversionError);
        }

        return Err(AssetError::AssetNotFound(format!(
            "Asset not found at: {}",
            asset_path.display()
        )));
    }

    // Verify it's a file, not a directory
    if !asset_path.is_file() {
        return Err(AssetError::InvalidPath(format!(
            "Path is not a file: {}",
            asset_path.display()
        )));
    }

    // Convert the path to a string
    asset_path
        .to_str()
        .map(|s| s.to_string())
        .ok_or(AssetError::PathConversionError)
}

/// Lists all assets in a given subdirectory.
///
/// # Arguments
/// * `subdirectory` - Optional subdirectory to list (e.g., "symbols"). If empty, lists root assets.
/// * `app_handle` - Tauri app handle for accessing the resource directory
///
/// # Returns
/// * `Ok(Vec<String>)` - List of relative asset paths
/// * `Err(AssetError)` - Error if the directory cannot be read
#[tauri::command]
fn list_assets(subdirectory: Option<String>, app_handle: tauri::AppHandle) -> Result<Vec<String>, AssetError> {
    let resource_dir = app_handle
        .path()
        .resource_dir()
        .map_err(|_| AssetError::ResourceDirNotFound)?;

    let assets_dir = resource_dir.join("assets");

    // Validate subdirectory path if provided
    if let Some(ref subdir) = subdirectory {
        if subdir.contains("..") {
            return Err(AssetError::InvalidPath(
                "Path traversal not allowed".to_string(),
            ));
        }
    }

    let target_dir = match &subdirectory {
        Some(subdir) => assets_dir.join(subdir),
        None => assets_dir.clone(),
    };

    if !target_dir.exists() {
        return Err(AssetError::AssetNotFound(format!(
            "Directory not found: {}",
            target_dir.display()
        )));
    }

    if !target_dir.is_dir() {
        return Err(AssetError::InvalidPath(format!(
            "Path is not a directory: {}",
            target_dir.display()
        )));
    }

    let mut assets = Vec::new();
    collect_assets(&target_dir, &assets_dir, &mut assets)?;

    Ok(assets)
}

/// Recursively collects asset paths from a directory
fn collect_assets(dir: &PathBuf, base_dir: &PathBuf, assets: &mut Vec<String>) -> Result<(), AssetError> {
    let entries = std::fs::read_dir(dir).map_err(|e| {
        AssetError::InvalidPath(format!("Failed to read directory: {}", e))
    })?;

    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_file() {
            // Get relative path from the assets base directory
            if let Ok(relative) = path.strip_prefix(base_dir) {
                if let Some(rel_str) = relative.to_str() {
                    assets.push(rel_str.replace('\\', "/"));
                }
            }
        } else if path.is_dir() {
            collect_assets(&path, base_dir, assets)?;
        }
    }

    Ok(())
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_fs::init())
        .plugin(tauri_plugin_shell::init())
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_window_state::Builder::new().build())
        .invoke_handler(tauri::generate_handler![get_asset_path, list_assets])
        .setup(|app| {
            // Start the Unix socket CLI bridge for debug JS execution
            start_cli_bridge(app.handle().clone());

            // Open devtools in debug builds
            #[cfg(debug_assertions)]
            {
                if let Some(window) = app.get_webview_window("main") {
                    window.open_devtools();
                }
            }
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

/// Starts a Unix socket listener that accepts JavaScript code snippets,
/// evaluates them in the main webview window, and returns the result.
///
/// This enables a REPL-style debug workflow:
///   echo 'document.title' | socat - UNIX-CONNECT:/tmp/nuxp-debug.sock
fn start_cli_bridge(app_handle: tauri::AppHandle) {
    use tokio::net::UnixListener;
    use tokio::io::{AsyncReadExt, AsyncWriteExt};

    // Tauri doesn't run a Tokio runtime, so we spawn a dedicated thread with its own runtime
    std::thread::spawn(move || {
        let rt = tokio::runtime::Runtime::new().expect("Failed to create Tokio runtime for CLI bridge");
        rt.block_on(async move {
            let socket_path = "/tmp/nuxp-debug.sock";
            let _ = std::fs::remove_file(socket_path);
            let listener = UnixListener::bind(socket_path).expect("Failed to bind debug socket");

            loop {
                if let Ok((mut socket, _)) = listener.accept().await {
                    let app_handle = app_handle.clone();
                    tokio::spawn(async move {
                        let mut buffer = [0; 4096];
                        if let Ok(n) = socket.read(&mut buffer).await {
                            if n == 0 { return; }
                            let js_code = String::from_utf8_lossy(&buffer[..n]).to_string();

                            // Create a unique ID for this request
                            let request_id = uuid::Uuid::new_v4().to_string();
                            let event_name = format!("debug-response-{}", request_id);

                            // Setup return channel
                            let (tx, rx) = tokio::sync::oneshot::channel::<String>();
                            let tx = std::sync::Arc::new(std::sync::Mutex::new(Some(tx)));

                            // Listen for the response event from the webview
                            let handler_id = app_handle.listen(event_name.clone(), move |event: tauri::Event| {
                                if let Ok(mut tx_guard) = tx.lock() {
                                    if let Some(tx) = tx_guard.take() {
                                        let _ = tx.send(event.payload().to_string());
                                    }
                                }
                            });

                            // Execute JS in the main window
                            if let Some(window) = app_handle.get_webview_window("main") {
                                // Wrap the JS to emit the result back via Tauri events
                                let script = format!(
                                    "
                                    (async () => {{
                                        try {{
                                            const result = eval({});
                                            window.__TAURI__.event.emit('{}', JSON.stringify(result));
                                        }} catch (e) {{
                                            window.__TAURI__.event.emit('{}', JSON.stringify({{ error: e.toString() }}));
                                        }}
                                    }})()
                                    ",
                                    serde_json::to_string(&js_code).unwrap(),
                                    event_name,
                                    event_name
                                );

                                if window.eval(&script).is_ok() {
                                    // Wait for response with timeout
                                    match tokio::time::timeout(std::time::Duration::from_secs(5), rx).await {
                                        Ok(Ok(response)) => {
                                            let _ = socket.write_all(response.as_bytes()).await;
                                            let _ = socket.write_all(b"\n").await;
                                        }
                                        Ok(Err(_)) => {
                                            let _ = socket.write_all(b"Error: Channel closed\n").await;
                                        }
                                        Err(_) => {
                                            let _ = socket.write_all(b"Error: Timeout waiting for JS execution\n").await;
                                        }
                                    }
                                } else {
                                    let _ = socket.write_all(b"Error: Failed to eval script\n").await;
                                }
                            } else {
                                let _ = socket.write_all(b"Error: Main window not found\n").await;
                            }

                            // Cleanup listener
                            app_handle.unlisten(handler_id);
                        }
                    });
                }
            }
        }); // end rt.block_on
    }); // end std::thread::spawn
}
