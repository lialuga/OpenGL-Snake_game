#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>  
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "Nem sikerült inicializálni a GLFW-t\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Nem sikerült létrehozni az ablakot\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
