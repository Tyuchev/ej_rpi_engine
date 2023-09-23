#include "config.h"
#include "debugdraw.h"

DebugDraw* DebugDraw::instance = nullptr;

DebugDraw* DebugDraw::Instance() {
    if (instance == nullptr) {
        instance = new DebugDraw();
    }
    return instance;
}

DebugDraw::DebugDraw() {
    ArrowStraightModel = Render::LoadModel("assets/system/ArrowStraight.glb");
    ArrowStraightOffsetModel = Render::LoadModel("assets/system/ArrowStraight.glb");
    ArrowRightModel = Render::LoadModel("assets/system/ArrowRightTurn.glb");
}

void DebugDraw::Draw() {
    for (const GameObject& object : objects) {
        object.Draw();
    }
}

// Returns angle between a and be in radians.
float AngleBetween(glm::vec3 a, glm::vec3 b) {
    glm::vec3 da = glm::normalize(a);
    glm::vec3 db = glm::normalize(b);
    return glm::acos(glm::dot(da, db));
}

void AdjustArrow(GameObject& arrow, glm::vec3 pos, glm::vec3 dir) {
    arrow.SetPos(pos);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
    dir = glm::normalize(dir);
    if (glm::length(dir) == 0) {
        dir = forward;
    }
    glm::vec3 cross = glm::cross(dir, forward);
    if (glm::length(cross) != 0) {
        float angle = AngleBetween(dir, forward);
        arrow.Rotate(cross, angle);
    }
    else if (dir == -forward) {
        arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1416f);
    }
}

void DebugDraw::ArrowStraight(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowStraightModel;
    // Rotate model to face correctly.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1416f);
    dir = dir * glm::vec3(-1.0f, 1.0f, 1.0f); // hack
    AdjustArrow(arrow, pos, dir);
    Instance()->objects.push_back(arrow);
}

void DebugDraw::ArrowStraightOffset(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowStraightOffsetModel;
    // Rotate model to face correctly.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1416f);
    dir = dir * glm::vec3(-1.0f, 1.0f, 1.0f); // hack
    AdjustArrow(arrow, pos, dir);
    Instance()->objects.push_back(arrow);
}

void DebugDraw::ArrowRight(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowRightModel;
    // Rotate model to face correctly.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1416f);
    dir = dir * glm::vec3(-1.0f, 1.0f, 1.0f); // hack
    AdjustArrow(arrow, pos, dir);
    Instance()->objects.push_back(arrow);
}

void DebugDraw::ArrowLeft(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowRightModel;
    // Rotate model to face correctly.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1416f);
    arrow.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 3.1416f);
    dir = dir * glm::vec3(1.0f, -1.0f, 1.0f); // hack
    AdjustArrow(arrow, pos, dir);
    Instance()->objects.push_back(arrow);
}