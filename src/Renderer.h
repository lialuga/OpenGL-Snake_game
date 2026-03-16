#pragma once

#include "Shader.h"
#include "Game.h"
#include "Wall.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const Game& game, float time);

private:
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void drawGrid() const;
    void drawTile(int gx, int gy, const glm::vec4& color, float padding) const;
};
