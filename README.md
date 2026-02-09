# NUXP ("Not UXP")

**No UXP? No problem.**

Adobe refuses to port UXP to Illustrator, leaving us stuck with the ancient, deprecated CEP framework from 2013.  
**NUXP** is a modern, sarcastic, and highly effective alternative bridge that lets you build native-feeling plugins using **Tauri**, **Rust**, and the **Adobe C++ SDK**, completely bypassing the limitations of CEP.

"If Adobe won't build it, we will."

## Architecture

NUXP replaces the rote "CEP Panel" with a standalone Tauri application that communicates directly with Illustrator via a persistent C++ plugin.

- **`cpp-plugin`**: A C++ plugin that loads into Illustrator and exposes a local HTTP server (or socket) for communication.
- **`app`**: A robust Tauri application (Rust + Web Frontend) that acts as the UI.
- **`host`**: Minimal ExtendScript (JSX) necessary for legacy DOM manipulation where the C++ SDK is too painful.

## Getting Started

*(Instructions coming soon)*
