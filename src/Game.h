#pragma once

#include "Snake.h"
#include "Wall.h"

#include <glm/glm.hpp>
#include <random>
#include <vector>

constexpr int GRID_W = 20;
constexpr int GRID_H = 20;

enum class FoodType { GOOD, BAD };

struct Food {
    int x;
    int y;
    FoodType type;
};

enum class GameState { PLAYING, GAME_OVER };

class Game {
public:
    Game();

    void update(float dt);
    void handleKey(int key);
    void handleMouseButton(int button, int action, glm::vec2 ndc);
    void handleMouseMove(glm::vec2 ndc);

    const Snake& getSnake() const { return snake; }
    const std::vector<Food>& getFoods() const { return foods; }
    const Wall& getWall() const { return wall; }
    int getLives() const { return lives; }
    int getScore() const { return score; }
    GameState getState() const { return state; }
    WallPhase getWallPhase() const { return wall.getPhase(); }

    void reset();

private:
    Snake snake;
    std::vector<Food> foods;
    Wall wall;
    int lives;
    int score;
    GameState state;

    float tickInterval;
    float tickTimer;

    std::mt19937 rng;

    void tick();
    void spawnFood();
    Food randomFood(FoodType type);
    bool isCellFree(int x, int y) const;
};
