#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>

constexpr int INVALID_MODEL_ID = 9009;

class GameObject {
public:
    bool isActive = true;

    GameObject();

    virtual void Draw();
    virtual void Update(float deltaTime);

    // TODO: Look over https://docs.unity3d.com/ScriptReference/Transform.html for reference
    void SetModel(const Render::ModelId& model);
    void SetPosition(glm::vec3 pos);
    void SetRotation(glm::quat quat);
    void Rotate(float radians, glm::vec3 axis);
    void AttachChild(const GameObject& child);
    void AttachParent(const GameObject& parent);


private:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 localPosition = glm::vec3(0);
    glm::quat orientation = glm::identity<glm::quat>();
    glm::quat localOrientation = glm::identity<glm::quat>();
    glm::mat4 transform = glm::mat4(1);
    Render::ModelId model;
    std::vector<GameObject> children;
    GameObject* parent;


};