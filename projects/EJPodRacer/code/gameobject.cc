#include "config.h"
#include "gameobject.h"
#include "render/renderdevice.h"

void GameObject::Translate(glm::vec3 dir) {
	transformMat = transformMat * glm::translate(dir);
}

// Rotates the object counterclockwise.
void GameObject::Rotate(glm::vec3 axis, float rad) {
	transformMat = transformMat * glm::rotate(rad, axis);
}

glm::vec3 GameObject::GetPos() const {
	// first three elements of last column
	return glm::vec3(transformMat[3][0], transformMat[3][1], transformMat[3][2]);
}

glm::vec3 GameObject::GetWorldPos() const {
    if (parent == nullptr)
        return GetPos();
    return parent->GetWorldPos() + GetPos();
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

void GameObject::AttachChild(GameObject* child) {
    this->children.push_back(child);
    child->AttachParent(this);
}

void GameObject::DetachChild(GameObject* child) {
    for (int i = 0; i < children.size(); i++) {
        if (&children[i] == &child) {
            children.erase(children.begin() + i);
            break;
        }
    }
}

void GameObject::DetachParent() {
    if (parent != nullptr) {
        parent->DetachChild(this);
        parent = nullptr;
    }
}

void GameObject::AttachParent(GameObject* parent) {
    this->DetachParent();
    this->parent = parent;
}


GameObject::GameObject() {
    model = INVALID_MODEL_ID;
    children = std::vector<GameObject*>();
    parent = nullptr;
}

GameObject::~GameObject() {
    for (GameObject* child : children) {
        delete child;
    }
    children.clear();
}

void GameObject::Draw() const {
    if (!isActive) 
        return;
    Render(glm::mat4(1.0f));
}

void GameObject::Render(glm::mat4 ctm) const {
    ctm = ctm * transformMat;
    if (model < INVALID_MODEL_ID) {
        // Children's scaling is independent of parent's for now.
        Render::RenderDevice::Draw(model, ctm * scalingMat, GetWorldPos(), isRoad);
    }
    for(GameObject* child : children) {
        if (child != nullptr)
        {
            child->Render(ctm);
        }
    }
}
