// @author zmertens
// A compute raytracer based on
// https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders

#include <cstdlib>
#include <stdexcept>

#include "Compute.hpp"

int main()
{
    std::cout << "Hello Compute" << std::endl;
    
    try {
        Compute compute;
        compute.run();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
