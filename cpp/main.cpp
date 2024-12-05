#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Function to get the current memory usage (platform-dependent)
#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
SIZE_T getProcessMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}
#elif defined(__linux__)
#include <unistd.h>
#include <sys/resource.h>
size_t getProcessMemoryUsage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss * 1024L; // ru_maxrss is in kilobytes
    }
    return 0;
}
#else
size_t getProcessMemoryUsage() {
    return 0;
}
#endif

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error: Could not initialize GLFW" << std::endl;
        return -1;
    }

    // Create an invisible window
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Memory Test", NULL, NULL);
    if (!window) {
        std::cerr << "Error: Could not create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    // Ignore the first error caused by glewInit
    glGetError();

    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Generate and bind texture
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Allocate texture data
    int width = 4096;
    int height = 4096;
    std::vector<unsigned char> data(width * height * 4, 255); // RGBA format

    std::cout << "Texture data memory: " << data.size() * 1.0 / 1024 / 1024 << " MB" << std::endl;

    // Measure memory before texture allocation
    size_t memBefore = getProcessMemoryUsage();
    std::cout << "Memory before glTexImage2D: " << memBefore * 1.0 / 1024 / 1024 << " MB" << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    // Measure memory after texture allocation
    size_t memAfter = getProcessMemoryUsage();
    std::cout << "Memory after glTexImage2D: " << memAfter * 1.0 / 1024 / 1024 << " MB" << std::endl;

    // Compute and display the difference
    size_t memDiff = memAfter - memBefore;
    std::cout << "Memory difference: " << memDiff * 1.0 / 1024 / 1024 << " MB" << std::endl;

    // Cleanup
    glDeleteTextures(1, &texID);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
