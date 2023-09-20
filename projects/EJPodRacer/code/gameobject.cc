#include "config.h"
#include "gameobject.h"
#include "render/renderdevice.h"

void Transform::Translate(glm::vec3 dir) {
	model = model * glm::translate(dir);
    for (Transform child : children) {
        child.model = child.model * model;
    }
}

void Transform::Rotate(glm::vec3 axis, float rad) {
	model = model * glm::rotate(rad, axis);
    for (Transform child : children) {
        child.model = child.model * model;
    }
}

glm::vec3 Transform::GetPos() {
	// first three elements of last column
	return glm::vec3(model[3][0], model[3][1], model[3][2]);
}

void Transform::SetPos(glm::vec3 pos) {
    // first three elements of last column
    model[3][0] = pos.x;
    model[3][1] = pos.y;
    model[3][2] = pos.z;
    for (Transform child : children) {
        child.model = child.model * model;
    }
}

void Transform::Scale(glm::vec3 amount) {
	scaling = glm::scale(amount);
}

void Transform::AttachChild(Transform& child) {
    this->children.push_back(child);
}

void Transform::DetachChild(Transform& child) {
    for (int i = 0; i < children.size(); i++) {
        if (&children[i] == &child) {
            children.erase(children.begin() + i);
            break;
        }
    }
}

void Transform::DetachParent() {
    if (parent != nullptr) {
        parent->DetachChild(*this);
        parent = nullptr;
    }
}

void Transform::AttachParent(Transform& parent) {
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
    // TODO: Is active?
    glm::mat4 mat = transform.model * transform.scaling;
    Render::RenderDevice::Draw(model, mat);
}
