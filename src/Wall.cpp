#include "Wall.h"

#include <OpenGL/gl3.h>

#include <algorithm>
#include <cmath>
#include <iostream>
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

Wall::Wall()
    : subdivisionLevel(0),
      dragIndex(-1),
      phase(WallPhase::EDITING),
      shader(shaderPath("vertex.glsl").c_str(), shaderPath("fragment.glsl").c_str()),
      VAO(0),
      VBO(0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Wall::~Wall() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Wall::addPoint(glm::vec2 p) {
    if (phase != WallPhase::EDITING) return;

    controlPoints.push_back(p);
    subdivisionLevel = 0;
    points = controlPoints;
    uploadToGPU();
}

void Wall::startDrag(glm::vec2 mouseNDC) {
    if (phase != WallPhase::EDITING) return;
    dragIndex = nearestControlPoint(mouseNDC);
}

void Wall::updateDrag(glm::vec2 mouseNDC) {
    if (dragIndex < 0 || phase != WallPhase::EDITING) return;

    controlPoints[dragIndex] = mouseNDC;
    points = controlPoints;
    for (int i = 0; i < subdivisionLevel; ++i) {
        points = catmullClarkStep(points);
    }
    uploadToGPU();
}

void Wall::endDrag() {
    dragIndex = -1;
}

std::vector<glm::vec2> Wall::catmullClarkStep(const std::vector<glm::vec2>& pts) {
    const int n = static_cast<int>(pts.size());
    if (n < 2) return pts;

    std::vector<glm::vec2> result;
    result.reserve(2 * n - 1);

    for (int i = 0; i < n; ++i) {
        if (i == 0 || i == n - 1) {
            result.push_back(pts[i]);
        } else {
            glm::vec2 newPt = (1.0f / 8.0f) * pts[i - 1]
                            + (6.0f / 8.0f) * pts[i]
                            + (1.0f / 8.0f) * pts[i + 1];
            result.push_back(newPt);
        }

        if (i < n - 1) {
            glm::vec2 edgeMid = 0.5f * pts[i] + 0.5f * pts[i + 1];
            result.push_back(edgeMid);
        }
    }

    return result;
}

void Wall::subdivide() {
    if (phase != WallPhase::EDITING) return;
    if (controlPoints.size() < 2) return;

    subdivisionLevel++;
    points = controlPoints;
    for (int i = 0; i < subdivisionLevel; ++i) {
        points = catmullClarkStep(points);
    }

    uploadToGPU();
    std::cout << "Catmull-Clark smoothing: level " << subdivisionLevel
              << " (" << points.size() << " points)\n";
}

void Wall::unsubdivide() {
    if (phase != WallPhase::EDITING) return;
    if (subdivisionLevel <= 0) return;

    subdivisionLevel--;
    points = controlPoints;
    for (int i = 0; i < subdivisionLevel; ++i) {
        points = catmullClarkStep(points);
    }

    uploadToGPU();
    std::cout << "Smoothing rollback: level " << subdivisionLevel
              << " (" << points.size() << " points)\n";
}

void Wall::finalize() {
    phase = WallPhase::PLAYING;
    std::cout << "Wall finalized. Game starts.\n";
}

void Wall::draw() const {
    if (points.empty() && controlPoints.empty()) return;

    shader.use();

    glm::vec4 lineColor = (phase == WallPhase::EDITING)
        ? glm::vec4(0.9f, 0.7f, 0.1f, 1.0f)
        : glm::vec4(0.9f, 0.4f, 0.1f, 1.0f);
    shader.setVec4("uColor", lineColor);

    glBindVertexArray(VAO);
    glLineWidth(3.0f);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(points.size()));

    if (phase == WallPhase::EDITING && !controlPoints.empty()) {
        shader.setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glPointSize(10.0f);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(controlPoints.size() * sizeof(glm::vec2)),
            controlPoints.data(),
            GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(controlPoints.size()));

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(points.size() * sizeof(glm::vec2)),
            points.data(),
            GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(0);
}

bool Wall::collides(glm::vec2 point, float threshold) const {
    if (points.size() < 2) return false;

    for (int i = 0; i < static_cast<int>(points.size()) - 1; ++i) {
        glm::vec2 a = points[i];
        glm::vec2 b = points[i + 1];
        glm::vec2 ab = b - a;
        glm::vec2 ap = point - a;

        float len2 = glm::dot(ab, ab);
        if (len2 < 1e-10f) continue;

        float t = glm::dot(ap, ab) / len2;
        t = std::max(0.0f, std::min(1.0f, t));

        glm::vec2 closest = a + t * ab;
        float dist = glm::length(point - closest);
        if (dist < threshold) {
            return true;
        }
    }

    return false;
}

void Wall::uploadToGPU() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(points.size() * sizeof(glm::vec2)),
        points.data(),
        GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Wall::nearestControlPoint(glm::vec2 p, float maxDist) const {
    int best = -1;
    float bestDist = maxDist;

    for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i) {
        float d = glm::length(controlPoints[i] - p);
        if (d < bestDist) {
            bestDist = d;
            best = i;
        }
    }

    return best;
}
