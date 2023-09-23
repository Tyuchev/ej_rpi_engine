#pragma once
#include "config.h"
#include "render/model.h"
#include "gameobject.h"
#include <vector>

class DebugDraw {
public:
    // Adds an arrow into the world to be drawn.
    static void ArrowStraight(glm::vec3 pos, glm::vec3 dir);
    // Adds an arrow into the world to be drawn. Arrow origin offset to be at base of arrow.
    static void ArrowStraightOffset(glm::vec3 pos, glm::vec3 dir);
    // Adds an arrow into the world to be drawn. Dir is from arrow base.
    static void ArrowRight(glm::vec3 pos, glm::vec3 dir);
    // Adds an arrow into the world to be drawn. Dir is from arrow base.
    static void ArrowLeft(glm::vec3 pos, glm::vec3 dir);

    void Draw();

    DebugDraw(DebugDraw& other) = delete;
    void operator=(const DebugDraw&) = delete;
    static DebugDraw* Instance();

private:
    Render::ModelId ArrowStraightModel;
    Render::ModelId ArrowRightModel;
    Render::ModelId ArrowStraightOffsetModel;

    DebugDraw();
    static DebugDraw* instance;

    std::vector<GameObject> objects;
};