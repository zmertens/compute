=== Compute ===

A raytracer algorithm on the OpenGL Compute shader. My goal is to allow interactive and non-interactive modes and do performance testing with a tool like Valgrind or Gperftools; particularly, I am interested in comparing results between GPU and CPU raytracers. These raytracers will be similar in features: Phong shading model, anti-aliasing, reflections, and refractions..


@author zmertens
@license BSD 3-Clause

Must specify Clang 12.0.0 compiler (C++14 support) or higher.

Build and Run: `cd build && cmake -G"Ninja" -DBUILD_SHARED_LIBS=true .. && ninja` and run with the glfw3.dll file next to the executable

References: https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders, https://learnopengl.com,
