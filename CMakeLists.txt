cmake_minimum_required(VERSION 3.10)
project(Snake)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(/opt/homebrew/include)

add_executable(Snake
	src/main.cpp
	src/Shader.cpp
	src/Snake.cpp
	src/Game.cpp
	src/Renderer.cpp
	src/Wall.cpp
)

target_compile_definitions(Snake PRIVATE
	SNAKE_SHADER_DIR="${CMAKE_SOURCE_DIR}/src/shaders"
)

target_link_libraries(Snake OpenGL::GL glfw)

add_custom_command(TARGET Snake POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/src/shaders
		$<TARGET_FILE_DIR:Snake>/shaders
)


