#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Game.h"
#include "Renderer.h"

constexpr int WIN_W = 600;
constexpr int WIN_H = 600;

static Game* activeGame = nullptr;
static int windowWidth = WIN_W;
static int windowHeight = WIN_H;

static glm::vec2 toNDC(double xpos, double ypos) {
    float x = static_cast<float>(xpos / windowWidth) * 2.0f - 1.0f;
    float y = -(static_cast<float>(ypos / windowHeight) * 2.0f - 1.0f);
    return {x, y};
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void key_callback(GLFWwindow* window, int key, int, int action, int) {
	if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}

	if (activeGame) {
		activeGame->handleKey(key);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
	if (!activeGame) return;
	double xpos = 0.0;
	double ypos = 0.0;
	glfwGetCursorPos(window, &xpos, &ypos);
	activeGame->handleMouseButton(button, action, toNDC(xpos, ypos));
}

void cursor_pos_callback(GLFWwindow*, double xpos, double ypos) {
	if (!activeGame) return;
	activeGame->handleMouseMove(toNDC(xpos, ypos));
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Snake", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glViewport(0, 0, WIN_W, WIN_H);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game game;
	Renderer renderer;
	activeGame = &game;

	float lastTime = static_cast<float>(glfwGetTime());

	while (!glfwWindowShouldClose(window)) {
		float now = static_cast<float>(glfwGetTime());
		float dt = now - lastTime;
		lastTime = now;

		game.update(dt);

		glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.draw(game, now);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
