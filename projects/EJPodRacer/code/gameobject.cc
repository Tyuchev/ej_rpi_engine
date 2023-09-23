#include "config.h"
#include "gameobject.h"
#include "render/renderdevice.h"

void GameObject::Translate(glm::vec3 dir) {
	transformMat = transformMat * glm::translate(dir);
}

void GameObject::Rotate(glm::vec3 axis, float rad) {
	transformMat = transformMat * glm::rotate(rad, axis);
}

glm::vec3 GameObject::GetPos() {
	// first three elements of last column
	return glm::vec3(transformMat[3][0], transformMat[3][1], transformMat[3][2]);
}

void GameObject::SetPos(glm::vec3 pos) {
    // first three elements of last column
    transformMat[3][0] = pos.x;
    transformMat[3][1] = pos.y;
    transformMat[3][2] = pos.z;
}

void GameObject::Scale(glm::vec3 amount) {
	scalingMat = glm::scale(amount);
}

void GameObject::AttachChild(GameObject& child) {
    this->children.push_back(child);
}

void GameObject::DetachChild(GameObject& child) {
    for (int i = 0; i < children.size(); i++) {
        if (&children[i] == &child) {
            children.erase(children.begin() + i);
            break;
        }
    }
}

void GameObject::DetachParent() {
    if (parent != nullptr) {
        parent->DetachChild(*this);
        parent = nullptr;
    }
}

void GameObject::AttachParent(GameObject& parent) {
    this->DetachParent();
    this->parent = &parent;
}


// ------------ GameObject ------------ 

GameObject::GameObject() {
    model = INVALID_MODEL_ID;
}

void GameObject::SetModel(const Render::ModelId& model) {
    this->model = model;
}

void GameObject::Draw() {
    if (!isActive) 
        return;
    Render(glm::mat4(1.0f));
}

void GameObject::Render(glm::mat4 ctm) {
    ctm = ctm * transformMat;
    // Children's scaling is independent of parent's for now.
    Render::RenderDevice::Draw(model, ctm * scalingMat);
    for(GameObject child : children) {
        child.Render(ctm);
    }
}
