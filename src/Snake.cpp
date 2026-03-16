#include "Snake.h"

Snake::Snake(int startX, int startY)
    : direction(Direction::RIGHT), nextDirection(Direction::RIGHT) {
    body.push_back({startX, startY});
    body.push_back({startX - 1, startY});
    body.push_back({startX - 2, startY});
}

void Snake::setDirection(Direction dir) {
    if (dir == Direction::UP && direction == Direction::DOWN) return;
    if (dir == Direction::DOWN && direction == Direction::UP) return;
    if (dir == Direction::LEFT && direction == Direction::RIGHT) return;
    if (dir == Direction::RIGHT && direction == Direction::LEFT) return;
    nextDirection = dir;
}

Segment Snake::step() {
    direction = nextDirection;

    Segment head = body.front();
    switch (direction) {
        case Direction::UP: head.y += 1; break;
        case Direction::DOWN: head.y -= 1; break;
        case Direction::LEFT: head.x -= 1; break;
        case Direction::RIGHT: head.x += 1; break;
    }

    Segment oldTail = body.back();
    body.pop_back();
    body.push_front(head);
    return oldTail;
}

void Snake::grow(Segment oldTail) {
    body.push_back(oldTail);
}

void Snake::wrapHead(int width, int height) {
    Segment& head = body.front();
    head.x = ((head.x % width) + width) % width;
    head.y = ((head.y % height) + height) % height;
}

bool Snake::collidesWithSelf() const {
    const Segment& head = body.front();
    for (int i = 1; i < static_cast<int>(body.size()); ++i) {
        if (body[i].x == head.x && body[i].y == head.y) {
            return true;
        }
    }
    return false;
}
