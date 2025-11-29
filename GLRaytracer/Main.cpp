// My raytracer with OpenGL 4 support and GPU raytracing

#include <cstdlib>

#include "Compute.hpp"

int main()
{
    try {
        Compute compute;
        compute.run();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
