# 3D Engine in OpenGL

![Screenshot 1](./media/screenshot.png)  
![Screenshot 2](./media/screenshot2.png)

## Overview

This project is a 3D engine built using OpenGL from scratch for Linux. It includes a basic map editor and supports loading 3D models, dynamic lighting, collision detection and etc.

## Features

- **Audio** with `OpenAL`
- **ImGui** debug windows
- **Sprite Animation**
- **Basic Map Editor**
  - Drawing static objects
  - Adding entities via JSON configuration
- **Entity System**
  - Pickups
  - Separate collision detection
- **Random Terrain Generation**
- **Model Loading**
  - Supports `.obj` format
- **Postprocessing Effects**
  - Pixelated (low-poly) view
  - Noise overlay
  - Illuminance color adjustment
  - Fog effects
  - Dynamic lighting
    - Flashlight
    - Multiple light sources
- **Text Rendering**
- **Collision Detection** using `Bullet3` physics engine

## Libraries Used

- OpenGL, GLAD, GLFW (graphics and window/context management)
- GLM (mathematics)
- ImGui (debugging and UI)
- stb_image (image loading)
- Assimp (model importing)
- Bullet3 (physics and collision detection)
- OpenAL (audio)

## Setup & Run

1. Install all required libraries (`glad`, `glm`, `imgui`, `KHR`,
   `stb_image`) and place them in the `libs` directory.
2. Compile and run by: `./run.sh`

