# 🔥 DOOM Fire Effect in C (SDL3)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)
![C](https://img.shields.io/badge/Language-C-blue.svg)
![SDL3](https://img.shields.io/badge/SDL-3-green.svg)
[![Build](https://img.shields.io/badge/Build-Makefile-orange.svg)](#-build--run)

A recreation of the iconic **DOOM Fire Effect** written in **pure C** using **SDL3** for rendering and **SDL3_ttf** for debug text.

---

## ✨ Features

- Classic DOOM PSX fire algorithm implementation
- Fire propagation with randomized decay
- Debug mode
- Optimized text rendering using texture cache
- Simple FPS system

---

## 📦 Requirements

To build and run this project on your machine, make sure you have the following installed:

- **GNU Make**
- **GCC**
- **SDL3 development libraries**
- **SDL3_ttf development libraries**
- **pkg-config**

---

## ⚙️ Installation

On Debian/Ubuntu-based systems:

```bash
sudo apt install gcc make libsdl3-dev libsdl3-ttf-dev pkg-config
```

On Fedora:

```bash
sudo dnf install gcc make SDL3-devel SDL3_ttf-devel pkg-config
```

On Arch Linux:

```bash
sudo pacman -S gcc make sdl3 sdl3_ttf pkg-config
```

---

## 📥 Clone the Repository

Clone this repository to your local machine using:

```bash
git clone https://github.com/lucaaszsx/doom-fire.git
cd doom-fire
```

---

## 🚀 Build & Run

Build:

```bash
make
```

Run:

```bash
make run
```

Clean:

```bash
make clean
```

---

## 🎨 Configuration

You can customize the fire effect by modifying the constants and algorithm in `src/main.c`:

### Display Settings

| Constant           | Description                    | Default |
|--------------------|--------------------------------|---------|
| `SCREEN_WIDTH`     | Window width in pixels         | 800                             |
| `SCREEN_HEIGHT`    | Window height in pixels        | 400                             |
| `MAP_ROWS`         | Number of vertical cells       | 80                              |
| `MAP_COLUMNS`      | Number of horizontal cells     | 100                             |
| `TARGET_FPS`       | Target frames per second       | 24.0                            |
| `MAX_INTENSITY_LEVEL` | Maximum fire intensity      | 36                              |
| `DEBUG_FONT_PATH`  | TTF debug font path            | /assets/fonts/YOU_FONT.ttf      |

### Fire Propagation Settings

You can adjust the fire behavior in the `calculate_propagation()` function:

```c
int vertical_decay = rand() % 3;
int horizontal_decay = rand() % 2;
```

---

## 🐛 Debug Mode

The project includes a debug mode that displays fire intensity values for each cell.

**Enable/Disable:**

Comment or uncomment the `#define DEBUG` line in `src/main.c`:

```c
#define DEBUG    // Enabled
// #define DEBUG // Disabled
```

---

## 🔬 Algorithm

The fire effect uses a cellular automaton approach:

1. **Base Layer**: Bottom row is set to maximum intensity
2. **Propagation**: Each cell copies intensity from the cell below
3. **Decay**: Random vertical decay reduces intensity
4. **Wind Effect**: Random horizontal displacement creates flickering
5. **Color Mapping**: Intensity values map to a 37-color fire palette

---

## 📜 License

This project is licensed under the **MIT License**.  
Do whatever you want with it, just give credit if you share.  
See **[LICENSE](./LICENSE)** for full license text.

---

## 🙏 Acknowledgments

- Original DOOM fire effect by **id Software**
- Algorithm explanation by **Fabien Sanglard**
- Inspired by the [DOOM Fire PSX](https://fabiensanglard.net/doom_fire_psx/) article

---

## 📸 Preview

