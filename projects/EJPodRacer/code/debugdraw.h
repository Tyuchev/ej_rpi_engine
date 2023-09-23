#pragma once
#include "config.h"
#include "render/model.h"
#include "gameobject.h"
#include <vector>

class DebugDraw {
public:
    // Adds an arrow into the world to be drawn.
    static void ArrowStraight(glm::vec3 pos, glm::vec3 dir);
    // Adds an arrow into the world to be drawn. Dir is head of arrow direction.
    static void ArrowRight(glm::vec3 pos, glm::vec3 dir);
    // Adds an arrow into the world to be drawn. Dir is head of arrow direction.
    static void ArrowLeft(glm::vec3 pos, glm::vec3 dir);

    void Draw();

    DebugDraw(DebugDraw& other) = delete;
    void operator=(const DebugDraw&) = delete;
    static DebugDraw* Instance();

private:
    Render::ModelId ArrowStraightModel;
    Render::ModelId ArrowRightModel;

    DebugDraw();
    static DebugDraw* instance;

    std::vector<GameObject> objects;
};