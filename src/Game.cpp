#include "Game.h"

#include <GLFW/glfw3.h>
#include <iostream>

Game::Game()
    : snake(GRID_W / 2, GRID_H / 2),
      lives(3),
      score(0),
      state(GameState::PLAYING),
      tickInterval(0.18f),
      tickTimer(0.0f),
      rng(std::random_device{}()) {
}

void Game::reset() {
    snake = Snake(GRID_W / 2, GRID_H / 2);
    lives = 3;
    score = 0;
    state = GameState::PLAYING;
    tickTimer = 0.0f;
    foods.clear();
    spawnFood();
}

void Game::handleKey(int key) {
    if (wall.getPhase() == WallPhase::EDITING) {
        if (key == GLFW_KEY_J) {
            wall.subdivide();
        } else if (key == GLFW_KEY_N) {
            wall.unsubdivide();
        } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
            wall.finalize();
            spawnFood();
            std::cout << "Game started. Lives: " << lives << "\n";
        }
        return;
    }

    if (state == GameState::GAME_OVER) {
        if (key == GLFW_KEY_R) {
            reset();
        }
        return;
    }

    switch (key) {
        case GLFW_KEY_UP: snake.setDirection(Direction::UP); break;
        case GLFW_KEY_DOWN: snake.setDirection(Direction::DOWN); break;
        case GLFW_KEY_LEFT: snake.setDirection(Direction::LEFT); break;
        case GLFW_KEY_RIGHT: snake.setDirection(Direction::RIGHT); break;
        default: break;
    }
}

void Game::handleMouseButton(int button, int action, glm::vec2 ndc) {
    if (wall.getPhase() != WallPhase::EDITING) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        wall.addPoint(ndc);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        wall.startDrag(ndc);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        wall.endDrag();
    }
}

void Game::handleMouseMove(glm::vec2 ndc) {
    if (wall.getPhase() != WallPhase::EDITING) return;
    wall.updateDrag(ndc);
}

void Game::update(float dt) {
    if (wall.getPhase() == WallPhase::EDITING) {
        return;
    }

    if (state != GameState::PLAYING) {
        return;
    }

    tickTimer += dt;
    if (tickTimer >= tickInterval) {
        tickTimer -= tickInterval;
        tick();
    }
}

void Game::tick() {
    Segment tailSegment = snake.step();
    snake.wrapHead(GRID_W, GRID_H);
    Segment head = snake.getBody().front();

    if (snake.collidesWithSelf()) {
        std::cout << "Self collision! Game over.\n";
        state = GameState::GAME_OVER;
        return;
    }

    float cellW = 2.0f / GRID_W;
    float cellH = 2.0f / GRID_H;
    glm::vec2 headNDC(
        -1.0f + (head.x + 0.5f) * cellW,
        -1.0f + (head.y + 0.5f) * cellH);
    if (wall.collides(headNDC)) {
        std::cout << "Wall collision! Game over.\n";
        state = GameState::GAME_OVER;
        return;
    }

    for (int foodIndex = 0; foodIndex < static_cast<int>(foods.size()); ++foodIndex) {
        if (foods[foodIndex].x == head.x && foods[foodIndex].y == head.y) {
            if (foods[foodIndex].type == FoodType::GOOD) {
                snake.grow(tailSegment);
                score++;
                std::cout << "Good food! Score: " << score << "\n";
            } else {
                lives--;
                std::cout << "Bad food! Lives: " << lives << "\n";
                if (lives <= 0) {
                    state = GameState::GAME_OVER;
                    return;
                }
            }

            foods.erase(foods.begin() + foodIndex);
            spawnFood();
            break;
        }
    }
}

void Game::spawnFood() {
    bool hasGood = false;
    bool hasBad = false;

    for (const auto& food : foods) {
        if (food.type == FoodType::GOOD) {
            hasGood = true;
        }
        if (food.type == FoodType::BAD) {
            hasBad = true;
        }
    }

    if (!hasGood) {
        foods.push_back(randomFood(FoodType::GOOD));
    }
    if (!hasBad) {
        foods.push_back(randomFood(FoodType::BAD));
    }
}

Food Game::randomFood(FoodType type) {
    std::uniform_int_distribution<int> distX(0, GRID_W - 1);
    std::uniform_int_distribution<int> distY(0, GRID_H - 1);

    Food food;
    food.type = type;

    int attempts = 0;
    do {
        food.x = distX(rng);
        food.y = distY(rng);
        attempts++;
    } while (!isCellFree(food.x, food.y) && attempts < 500);

    return food;
}

bool Game::isCellFree(int x, int y) const {
    for (const auto& segment : snake.getBody()) {
        if (segment.x == x && segment.y == y) {
            return false;
        }
    }

    for (const auto& food : foods) {
        if (food.x == x && food.y == y) {
            return false;
        }
    }

    return true;
}
