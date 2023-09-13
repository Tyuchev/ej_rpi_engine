#include "config.h"
#include "gameobject.h"


GameObject::GameObject() {
    model = INVALID_MODEL_ID;
}


void GameObject::SetModel(const Render::ModelId& model) {
    this->model = model;
}

void GameObject::SetPosition(glm::vec3 pos) {

}

void GameObject::SetRotation(glm::quat quat) {

}

void GameObject::Rotate(float radians, glm::vec3 axis) {
    
}

void GameObject::AttachChild(const GameObject& child) {

}

void GameObject::AttachParent(const GameObject& parent) {

}

void GameObject::Draw() {

}

void GameObject::Update(float deltaTime) {

}