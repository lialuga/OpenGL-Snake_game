#define GL_SILENCE_DEPRECATION  
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>

int main() {
 
    if (!glfwInit()) {
        std::cerr << "Hiba: GLFW inicializálás nem sikerült!" << std::endl;
        return -1;
    }

 
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Hiba: ablak létrehozása nem sikerült!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };


        glfwSwapBuffers(window); 
        glfwPollEvents();       
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
