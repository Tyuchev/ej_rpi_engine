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
    ArrowRightModel = Render::LoadModel("assets/system/ArrowRightTurn.glb");
}

void DebugDraw::Draw() {
    //for (const GameObject& object : objects) {
        //object.Draw();
    //}
}

// Returns angle between a and be in radians.
float AngleBetween(glm::vec3 a, glm::vec3 b) {
    glm::vec3 da = glm::normalize(a);
    glm::vec3 db = glm::normalize(b);
    return glm::acos(glm::dot(da, db));
}

void DebugDraw::ArrowStraight(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowStraightModel;
    arrow.SetPos(pos);
    // Reverse direction to be correct.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.1415f);
    arrow.Rotate(glm::cross(dir, glm::vec3(0.0f, 0.0f, 1.0f)), AngleBetween(glm::vec3(0.0f, 0.0f, 1.0f), dir));
    Instance()->objects.push_back(arrow);
}

void DebugDraw::ArrowRight(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowRightModel;
    arrow.SetPos(pos);
    // Offset by 90 deg to account from model orientation.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -1.5707f);
    arrow.Rotate(glm::cross(dir, glm::vec3(0.0f, 0.0f, 1.0f)), AngleBetween(glm::vec3(0.0f, 0.0f, 1.0f), dir));
    Instance()->objects.push_back(arrow);
}

void DebugDraw::ArrowLeft(glm::vec3 pos, glm::vec3 dir) {
    GameObject arrow;
    arrow.model = Instance()->ArrowRightModel;
    arrow.SetPos(pos);
    // Offset by 180 deg to turn right turn into left turn.
    arrow.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 3.1415f);
    // Offset by 90 deg to account from model orientation.
    arrow.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.5707f);
    arrow.Rotate(glm::cross(dir, glm::vec3(0.0f, 0.0f, 1.0f)), AngleBetween(glm::vec3(0.0f, 0.0f, 1.0f), dir));
    Instance()->objects.push_back(arrow);
}