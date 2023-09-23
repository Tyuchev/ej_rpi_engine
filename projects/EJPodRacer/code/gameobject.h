#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>

constexpr int INVALID_MODEL_ID = 9009;

//struct Transform {
	//glm::mat4 model = glm::mat4(1);
	//glm::mat4 scaling = glm::mat4(1);

    //std::vector<Transform> children;
    //Transform* parent;

	//void Translate(glm::vec3 dir);
	//void Rotate(glm::vec3 axis, float rad);
	//glm::vec3 GetPos();
    //void SetPos(glm::vec3 pos);
	//void Scale(glm::vec3 amount);
    //void AttachChild(Transform& child);
    //void DetachChild(Transform& child);
    //void DetachParent();
    //void AttachParent(Transform& parent);
//};


class GameObject {

public:
    Render::ModelId model;
    std::vector<GameObject> children;
    GameObject* parent;

	void Translate(glm::vec3 dir);
	void Rotate(glm::vec3 axis, float rad);
	glm::vec3 GetPos();
    void SetPos(glm::vec3 pos);
	void Scale(glm::vec3 amount);
    void AttachChild(GameObject & child);
    void DetachChild(GameObject & child);
    void DetachParent();
    void AttachParent(GameObject & parent);

    bool isActive = true;

    GameObject();

    virtual void Draw();
    virtual void Update(float deltaTime) {};

    void SetModel(const Render::ModelId& model);


private:
    void Render(glm::mat4 ctm);
	glm::mat4 transformMat = glm::mat4(1.0f);
	glm::mat4 scalingMat = glm::mat4(1.0f);
};