# 🏡 3D House Visualization (C++ / OpenGL)

An interactive 3D house simulation created in C++ using OpenGL and GLUT. The project features realistic room structures, textured surfaces, dynamic lighting, and camera navigation, allowing users to explore a virtual house environment.

---

## 🎯 Features

- 🔭 **3D Visualization of a Complete House**: Includes living room, kitchen, bedrooms, bathroom, dining area, aisles, roof, and outdoor garden.
- 🪟 **Multiple Views**: Main 3D exploration window + 2D projection views.
- 🖱️ **User Interaction**: Navigate the camera using keyboard input (`F`, `B`, `R`, `L`, `U`, `D`).
- 🌱 **Procedural Terrain**: Dynamic grass field and curved sky dome using Bézier surfaces.
- 🧱 **Realistic Textures**: Wood, grass, sky, wall, carpet, etc. loaded from BMP files.
- ☀️ **Lighting & Materials**: Ambient, diffuse, and specular lighting for realism.
- 🛋️ **Custom 3D Furniture**: Sofa, beds, tables, chairs, plant pots, trees, and more.
- 🛠️ **Modular Code**: Each part of the house (e.g. `drawKitchen`, `drawRoof`, `drawTree`) is organized in reusable functions.

---

## 🧠 Technologies Used

- **C++**
- **OpenGL / GLUT**
- **Bezier Surface Generation**
- **Keyboard Interaction**
- **Material & Light Management**
- **Makefile for build automation**

---

## 🧭 Controls

| Key | Action               |
|-----|----------------------|
| `F` | Move forward         |
| `B` | Move backward        |
| `R` | Turn right           |
| `L` | Turn left            |
| `U` | Move camera up       |
| `D` | Move camera down     |
| `Q` | Quit the application |

---

## 🔧 How to Compile and Run (Linux)

```bash
# Compile both programs
make

# Run main visualization
./visiting3Dhouse

# Run projection views
./views
