#include "Renderer.h"

#include <OpenGL/gl3.h>
#include <cmath>
#include <string>

namespace {
std::string shaderPath(const char* fileName) {
#ifdef SNAKE_SHADER_DIR
    return std::string(SNAKE_SHADER_DIR) + "/" + fileName;
#else
    return std::string("shaders/") + fileName;
#endif
}
}

static const unsigned int quadIndices[] = {
    0, 1, 2,
    0, 2, 3
};

Renderer::Renderer()
    : shader(shaderPath("vertex.glsl").c_str(), shaderPath("fragment.glsl").c_str()) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::draw(const Game& game, float time) {
    drawGrid();

    game.getWall().draw();

    if (game.getWallPhase() == WallPhase::EDITING) {
        return;
    }

    for (const auto& food : game.getFoods()) {
        float pulse = 0.06f + 0.04f * static_cast<float>(std::sin(time * 4.0f));
        glm::vec4 color = (food.type == FoodType::GOOD)
            ? glm::vec4(0.95f, 0.20f, 0.20f, 1.0f)
            : glm::vec4(0.20f, 0.45f, 0.95f, 1.0f);
        drawTile(food.x, food.y, color, pulse);
    }

    const auto& body = game.getSnake().getBody();
    for (int i = 0; i < static_cast<int>(body.size()); ++i) {
        float nx = static_cast<float>(body[i].x) / static_cast<float>(GRID_W - 1);
        float ny = static_cast<float>(body[i].y) / static_cast<float>(GRID_H - 1);

        float r = 0.1f + 0.7f * nx;
        float g = (i == 0) ? 0.95f : 0.75f;
        float b = 0.1f + 0.6f * ny;

        float pad = (i == 0) ? 0.02f : 0.05f;
        drawTile(body[i].x, body[i].y, glm::vec4(r, g, b, 1.0f), pad);
    }

    if (game.getState() == GameState::GAME_OVER) {
        float alpha = 0.4f + 0.4f * static_cast<float>(std::sin(time * 6.0f));
        for (int x = 0; x < GRID_W; ++x) {
            drawTile(x, 0, glm::vec4(0.9f, 0.1f, 0.1f, alpha), 0.0f);
            drawTile(x, GRID_H - 1, glm::vec4(0.9f, 0.1f, 0.1f, alpha), 0.0f);
        }
        for (int y = 1; y < GRID_H - 1; ++y) {
            drawTile(0, y, glm::vec4(0.9f, 0.1f, 0.1f, alpha), 0.0f);
            drawTile(GRID_W - 1, y, glm::vec4(0.9f, 0.1f, 0.1f, alpha), 0.0f);
        }
    }
}

void Renderer::drawGrid() const {
    for (int x = 0; x < GRID_W; ++x) {
        for (int y = 0; y < GRID_H; ++y) {
            float shade = ((x + y) % 2 == 0) ? 0.13f : 0.11f;
            drawTile(x, y, glm::vec4(shade, shade, shade, 1.0f), 0.01f);
        }
    }
}

void Renderer::drawTile(int gx, int gy, const glm::vec4& color, float padding) const {
    float cellW = 2.0f / GRID_W;
    float cellH = 2.0f / GRID_H;

    float x0 = -1.0f + gx * cellW + padding * cellW;
    float y0 = -1.0f + gy * cellH + padding * cellH;
    float x1 = x0 + cellW * (1.0f - 2.0f * padding);
    float y1 = y0 + cellH * (1.0f - 2.0f * padding);

    float verts[] = {
        x0, y0,
        x1, y0,
        x1, y1,
        x0, y1
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader.use();
    shader.setVec4("uColor", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
