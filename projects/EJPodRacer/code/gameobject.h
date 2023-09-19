#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>

constexpr int INVALID_MODEL_ID = 9009;

struct Transform {
	glm::mat4 model;
	glm::mat4 scaling;

    std::vector<Transform> children;
    Transform* parent;

	void Translate(glm::vec3 dir);
	void Rotate(glm::vec3 axis, float rad);
	glm::vec3 GetPos();
	void Scale(glm::vec3 amount);
    void AttachChild(Transform& child);
    void DetachChild(Transform& child);
    void DetachParent();
    void AttachParent(Transform& parent);
};


class GameObject {
public:
    bool isActive = true;
    Transform transform;

    GameObject();

    virtual void Draw();
    virtual void Update(float deltaTime) {};

    void SetModel(const Render::ModelId& model);


private:
    Render::ModelId model;
    std::vector<GameObject> children;
    GameObject* parent;
};