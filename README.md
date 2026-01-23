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

You can customize the fire effect by modifying the constants in `src/main.c`:

### Display Settings

| Constant           | Description                    | Default                         |
|--------------------|--------------------------------|---------------------------------|
| `APP_TITLE`        | Window title                   | "DOOM Fire Effect"              |
| `DEBUG_FONT_PATH`  | Debug font path                | /assets/fonts/YOUR_FONT.ext     |
| `TARGET_FPS`       | Targer frames per second       | 24                              |
| `SCREEN_WIDTH`     | Window width in pixels         | 800                             |
| `SCREEN_HEIGHT`    | Window height in pixels        | 400                             |
| `MAP_ROWS`         | Number of vertical cells       | 80                              |
| `MAP_COLUMNS`      | Number of horizontal cells     | 100                             |
| `TARGET_FPS`       | Target frames per second       | 24.0                            |
| `MAX_INTENSITY_LEVEL` | Maximum fire intensity      | 36                              |
| `FIRE_DECAY`       | Fire decay (vertical effect)   | 3                               |
| `FIRE_SPREAD`      | Fire spread (horizontal effect)| 2                               |

### 🔥 Fire settings

You don't need to do changes to fire settings, but you can. If you want to see how the fire effect will behave with different decay and spread values, just change the `FIRE_DECAY` and `FIRE_SPREAD` constants.

### ✏️ Font Settings

If you're not going to use debug mode, you can ignore this topic. Otherwise, debug mode won't work without this configuration. If you want to use debug mode, you will need to use some font to render text. To configure the font, you can follow these steps:

1. Download some font in TTF/OTF/TTC format
2. Move the font file to the project folder
3. Change the `DEBUG_FONT_PATH` at `src/main.c` to your font file path
4. Follow the steps in topic **[Debug Mode](#debug-mode)** to configure debug mode

## 🐛 Debug Mode

The project includes a debug mode that displays fire intensity values for each cell. Isn't recommended to use debug mode when you have many cells. 

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
