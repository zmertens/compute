# Compute

A simple project demonstrating a basic raytracer algorithm which is parallelized by OpenGL compute shaders.

Performance results may vary depending on your hardware, but generally, using compute shaders can significantly speed up raytracing tasks compared to CPU-based implementations.

## CMake

This project uses CMake as its build system. To build the project, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/zmertens/compute.git --recurse-submodules
   cd compute
   ```
   
2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```
   
3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```

## Learning Materials

  - https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders
  - https://learnopengl.com
