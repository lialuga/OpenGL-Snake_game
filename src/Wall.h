#pragma once

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.h"

enum class WallPhase {
    EDITING,
    PLAYING
};

class Wall {
public:
    Wall();
    ~Wall();

    void addPoint(glm::vec2 p);
    void startDrag(glm::vec2 mouseNDC);
    void updateDrag(glm::vec2 mouseNDC);
    void endDrag();

    void subdivide();
    void unsubdivide();

    void finalize();

    WallPhase getPhase() const { return phase; }

    void draw() const;

    bool collides(glm::vec2 pointNDC, float threshold = 0.04f) const;

    const std::vector<glm::vec2>& getPoints() const { return points; }

private:
    std::vector<glm::vec2> controlPoints;
    std::vector<glm::vec2> points;

    int subdivisionLevel;
    int dragIndex;

    WallPhase phase;

    Shader shader;
    unsigned int VAO;
    unsigned int VBO;

    void uploadToGPU() const;

    static std::vector<glm::vec2> catmullClarkStep(const std::vector<glm::vec2>& pts);

    int nearestControlPoint(glm::vec2 p, float maxDist = 0.08f) const;
};
