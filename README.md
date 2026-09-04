# Screenshot Translator

![Status: Work in Progress](https://img.shields.io/badge/Status-Work_in_Progress-orange) 
![Language: C](https://img.shields.io/badge/Language-C-blue) 
![License: GPL v3.0](https://img.shields.io/badge/license-GPL--3.0-green.svg)

[![EN](https://img.shields.io/badge/lang-English-blue)](README.md) [![UK](https://img.shields.io/badge/мова-Українська-yellow)](README.uk.md)

> **Note:** This project is currently in active development (Work in Progress).

This project is a rework of my old pet project which was written in Python using PyQt5.
I was unsatisfied with the performance and struggled to deploy that translator. Because of that, I decided to completely rewrite it in C using a low-level graphics library, to learn what benefits low-level development can offer. The main purpose is to adapt my previous work into a new, fast, and easy-to-deploy environment — without using high-level toolkits like GTK or Qt.

## 🚀 Current Features

At this stage, basic interaction with the display server and rendering has been implemented:
* Connecting to the Wayland compositor (`wl_display_connect`).
* Initialization and handling of global objects for input peripherals and interface rendering.
* Creating a window using the `xdg_shell` extension.
* Rendering the image as a 1D array to serve as the interface.


![Interface](/window_data/Images/neo_futuristic-back.png)


## 🗺️ Roadmap

# Main goals:
   - [ ] 1. Build the main window that will handle all interactions with the user.
   - [ ] 2. Create a logical architecture that will perform all OCR tasks.
   - [ ] 3. Port over all the features from the old Python version.

## 1. Building the main window 🖼️
### Phase 1: Basic architecture and rendering (Completed) ✅
- [x] Set up basic window creation using the Wayland protocol.
- [x] Create a function to work with buffers and draw pixels into them.
- [x] Add the ability to output an image to the interface via a buffer.
- [x] Draw the image with the translator interface.

### Phase 2: Input handling (In Progress) ⏳
- [x] Obtain the global `wl_seat` object and set up handling of its events.
- [x] Connect `wl_pointer` and handle mouse movement and clicks.
- [ ] Connect `wl_keyboard` and handle key presses.
- [ ] Verify manual text input works correctly.
- [ ] Create a universal function for creating and handling buttons.
- [ ] Create a universal function for creating and handling text fields.

### Phase 3: Improving interface appearance and responsiveness
- [ ] Formatting: adjust text appearance for specific conditions of the drawn interface.
- [ ] Dynamic resizing and scaling of the interface when the window is resized.
- [ ] Button animations and their reaction to user interaction.

## 2. Building the logical architecture ⚙️
### Phase 1: Integrating OCR tools and a screenshot tool
- [ ] Add the ability to take screenshots via grim + slurp.
- [ ] Integrate a local, lightweight OCR tool — Tesseract.
- [ ] Integrate a powerful, modern recognition tool — PaddleOCR. (web API)
- [ ] Improve text recognition through local image preprocessing.

### Phase 2: Integrating translation services
- [ ] Add support for various popular text translation APIs.
- [ ] Implement logic for processing input text and sending it for translation.

## 3. Porting over old features ✨
- [ ] Create a settings window where the translator's behavior can be configured, and default API/languages can be selected.
- [ ] Implement the logic for the settings window.
- [ ] Consider restoring the ability to create custom themes for the user.

## 🛠 Build and run: (Current)

**Dependencies:**
- `gcc`
- `wayland`
* `wayland-protocols`

**Compilation:**
```bash
gcc ./window.c ./window_data/glue_code/xdg-shell.c -o window -lwayland-client
```
