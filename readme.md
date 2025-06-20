# 3D Engine in OpenGL

![Screenshot 1](https://github.com/L0puh/3Dengine/blob/master/media/screenshot.png)  
![Screenshot 2](https://github.com/L0puh/3Dengine/blob/master/media/screenshot2.png)

## Overview

This project is a 3D engine built using OpenGL from scratch. It includes a basic map editor and supports loading 3D models, dynamic lighting, and collision detection.

## Features

- **Audio** with `OpenAL`
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
