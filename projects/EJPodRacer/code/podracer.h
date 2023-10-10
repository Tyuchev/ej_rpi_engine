#pragma once
#include "render/model.h"
#include "render/physics.h"
#include "render/input/inputserver.h"
#include "render/cameramanager.h"
#include "render/debugrender.h"
#include "render/particlesystem.h"

#include <vector>
#include <tuple>
#include <iostream>


namespace Render
{
    struct ParticleEmitter;
}

namespace Game
{

enum class ControlScheme {
    NoControls,
    NewControls,
    DebugControls
};

struct PodRacer
{
    PodRacer();
    
    glm::vec3 position = glm::vec3(0);
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec3 camPos = glm::vec3(0, 1.0f, -2.0f);
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 linearVelocity = glm::vec3(0);
    ControlScheme controlScheme = ControlScheme::NewControls;

    const float normalSpeed = 1.0f;
    const float maxSpeed = normalSpeed * 2.0f;
    const float acceleration = 1.0f;
    const float camOffsetY = 2.0f;
    const float cameraSmoothFactor = 10.0f;

    float currentSpeed = 0.0f;

    float rotationZ = 0;
    float rotXSmooth = 0;
    float rotYSmooth = 0;
    float rotZSmooth = 0;

    Render::ModelId model;

    Physics::ColliderId colliderID;

    void Update(float dt);

    void CheckCollisions(std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> &collisions);
    void ResolveCollisions(std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> &collisions) const;

private:
    void ApplyNewControls(const float& dt);
    void ApplyDebugControls(const float& dt);
    void ApplyNoControls(const float& dt);
    

    // Hard-coded collision points from the Engine's test space simulation
    //
    // const glm::vec3 colliderEndPoints[8] = {
    //     glm::vec3(-1.10657, -0.480347, -0.346542),  // right wing
    //     glm::vec3(1.10657, -0.480347, -0.346542),  // left wing
    //     glm::vec3(-0.342382, 0.25109, -0.010299),   // right top
    //     glm::vec3(0.342382, 0.25109, -0.010299),   // left top
    //     glm::vec3(-0.285614, -0.10917, 0.869609), // right front
    //     glm::vec3(0.285614, -0.10917, 0.869609), // left front
    //     glm::vec3(-0.279064, -0.10917, -0.98846),   // right back
    //     glm::vec3(0.279064, -0.10917, -0.98846)   // right back
    // };
};

}