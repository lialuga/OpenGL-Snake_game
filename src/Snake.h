#pragma once

#include <deque>

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Segment {
    int x;
    int y;
};

class Snake {
public:
    Snake(int startX, int startY);

    void setDirection(Direction dir);
    Direction getDirection() const { return direction; }

    Segment step();
    void grow(Segment tailSegment);
    void wrapHead(int width, int height);

    int length() const { return static_cast<int>(body.size()); }
    const std::deque<Segment>& getBody() const { return body; }

    bool collidesWithSelf() const;

private:
    std::deque<Segment> body;
    Direction direction;
    Direction nextDirection;
};
