#include "config.h"
#include "podracer.h"
#include "render/input/inputserver.h"
#include "render/cameramanager.h"
#include "render/physics.h"
#include "render/debugrender.h"
#include "render/particlesystem.h"

#include <vector>
#include <tuple>
#include <iostream>

using namespace Input;
using namespace glm;
using namespace Render;

namespace Game
{
PodRacer::PodRacer()
{
    uint32_t numParticles = 2048;
    this->particleEmitterLeft = new ParticleEmitter(numParticles);
    this->particleEmitterLeft->data = {
        .origin = glm::vec4(this->position + (vec3(this->transform[2]) * emitterOffset),1),
        .dir = glm::vec4(glm::vec3(-this->transform[2]), 0),
        .startColor = glm::vec4(0.38f, 0.76f, 0.95f, 1.0f) * 2.0f,
        .endColor = glm::vec4(0,0,0,1.0f),
        .numParticles = numParticles,
        .theta = glm::radians(0.0f),
        .startSpeed = 1.2f,
        .endSpeed = 0.0f,
        .startScale = 0.025f,
        .endScale = 0.0f,
        .decayTime = 2.58f,
        .randomTimeOffsetDist = 2.58f,
        .looping = 1,
        .emitterType = 1,
        .discRadius = 0.020f
    };
    this->particleEmitterRight = new ParticleEmitter(numParticles);
    this->particleEmitterRight->data = this->particleEmitterLeft->data;

    ParticleSystem::Instance()->AddEmitter(this->particleEmitterLeft);
    ParticleSystem::Instance()->AddEmitter(this->particleEmitterRight);
}

void
PodRacer::Update(float dt)
{
    Mouse* mouse = Input::GetDefaultMouse();
    Keyboard* kbd = Input::GetDefaultKeyboard();

    Camera* cam = CameraManager::GetCamera(CAMERA_MAIN);

    // 'dt' is either the delta time per frame or is a minimum value of 0.04..

    // Forward movement
    // movement seems to be smoothed by using GLM::mix
    
    if(kbd->held[Key::S])
    {

        this->currentSpeed = mix(this->currentSpeed, -1.0f, std::min(1.0f, dt * acceleration));

    }
    // braking takes precedence - as above
    else if (kbd->held[Key::W])
    {

        this->currentSpeed = mix(this->currentSpeed, this->normalSpeed, std::min(1.0f, dt * acceleration));

        // An old "BOOST" mechanic
        // if (kbd->held[Key::Shift])
        //     this->currentSpeed = mix(this->currentSpeed, this->maxSpeed, std::min(1.0f, dt * 30.0f));
        // else
        //     this->currentSpeed = mix(this->currentSpeed, this->normalSpeed, std::min(1.0f, dt * 90.0f));

    }
    else
    {
        if(this->currentSpeed < this->normalSpeed * 0.1)
        {
            this->currentSpeed = 0;
        }
        else
        {
            this->currentSpeed = 0.5 * this->currentSpeed;
        }
    }

    vec3 desiredVelocity = vec3(0, 0, this->currentSpeed);
    desiredVelocity = this->transform * vec4(desiredVelocity, 0.0f);

    this->linearVelocity = mix(this->linearVelocity, desiredVelocity, dt);
    this->position += this->linearVelocity * dt * 10.0f;

    // Rotations - OLD ROTATION SYSTEM
    // float rotX = kbd->held[Key::Left] ? 1.0f : kbd->held[Key::Right] ? -1.0f : 0.0f;
    // float rotY = kbd->held[Key::Up] ? -1.0f : kbd->held[Key::Down] ? 1.0f : 0.0f;
    // float rotZ = kbd->held[Key::A] ? -1.0f : kbd->held[Key::D] ? 1.0f : 0.0f;
    // const float rotationSpeed = 1.8f * dt;

    // movement seems to be smoothed by using GLM::mix
    // rotXSmooth = mix(rotXSmooth, rotX * rotationSpeed, dt * cameraSmoothFactor);
    // rotYSmooth = mix(rotYSmooth, rotY * rotationSpeed, dt * cameraSmoothFactor);
    // rotZSmooth = mix(rotZSmooth, rotZ * rotationSpeed, dt * cameraSmoothFactor);


        // THIS COULD BE USED TO MAKE A NICE 'TILTING' ANIMATION WHEN PLAYER 'BANKS' LEFT/RIGHT

    // quat localOrientation = quat(vec3(-rotYSmooth, rotXSmooth, rotZSmooth));

    // TO DO - ADD IN DT TO SMOOTH OUT ANIMATION - 45 degree
    
    
    float bankingDirection = 0;
    float turnSpeed = 2.0f * dt;
    kbd->held[Key::A] ? bankingDirection = -1.0f : kbd->held[Key::D] ? bankingDirection = 1.0f : bankingDirection = 0.0f;
    bankingDirection *= turnSpeed;
    float rotZ = bankingDirection;

    // Ensures that rotation cannot go past X degrees
    if (rotationZ > 29.0f && bankingDirection > 0)
    {
        rotZ = 0;
    }
    else if(rotationZ < -29.0f && bankingDirection < 0)
    {
        rotZ = 0;
    }

    this->rotationZ += rotZ / dt;
    this->rotationZ = clamp(this->rotationZ, -30.0f, 30.0f);
    quat localOrientation = quat(vec3(0, 0, rotZ));
    this->orientation = this->orientation * localOrientation;

    // Set translation matrix by takeing position * orientation
    mat4 T = translate(this->position) * (mat4)this->orientation;
    this->transform = T * (mat4)quat(vec3(0, 0, rotZ));
    //this->rotationZ = mix(this->rotationZ, 0.0f, dt * cameraSmoothFactor);

    // update camera view transform
    // camera is offset by (0, camOffsetY, -4.0) 
    vec3 desiredCamPos = this->position + vec3(this->transform * vec4(0, camOffsetY, -4.0f, 0));
    this->camPos = mix(this->camPos, desiredCamPos, dt * cameraSmoothFactor);
    cam->view = lookAt(this->camPos, this->position + vec3(this->transform[2]) * 50.0f, vec3(this->transform[1]));


    // Particle emitters - OFFLINE - I dont think these are working/full hooked-up
    //
    //
    // const float thrusterPosOffset = 0.365f;
    // this->particleEmitterLeft->data.origin = glm::vec4(vec3(this->position + (vec3(this->transform[0]) * -thrusterPosOffset)) + (vec3(this->transform[2]) * emitterOffset), 1);
    // this->particleEmitterLeft->data.dir = glm::vec4(glm::vec3(-this->transform[2]), 0);
    // this->particleEmitterRight->data.origin = glm::vec4(vec3(this->position + (vec3(this->transform[0]) * thrusterPosOffset)) + (vec3(this->transform[2]) * emitterOffset), 1);
    // this->particleEmitterRight->data.dir = glm::vec4(glm::vec3(-this->transform[2]), 0);
    
    // float t = (currentSpeed / this->normalSpeed);
    // this->particleEmitterLeft->data.startSpeed = 1.2 + (3.0f * t);
    // this->particleEmitterLeft->data.endSpeed = 0.0f  + (3.0f * t);
    // this->particleEmitterRight->data.startSpeed = 1.2 + (3.0f * t);
    // this->particleEmitterRight->data.endSpeed = 0.0f + (3.0f * t);
    // this->particleEmitter->data.decayTime = 0.16f;//+ (0.01f  * t);
    // this->particleEmitter->data.randomTimeOffsetDist = 0.06f;/// +(0.01f * t);
}

void
PodRacer::CheckCollisions(std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> &collisions)
{
    glm::mat4 rotation = (glm::mat4)orientation;
    
    // iterating to 8 in below loop was hard coded - 
    // It raycast from each outer point of the original model

    // Change to 10 x 10 rays across whatever model we load?
    // Or maybe 1 ray every 'x' pixels?
    for (int i = 0; i < 1; i++)
    {
        glm::vec3 pos = position;
        //Maybe normalize 'pos'
        glm::vec3 dir = rotation * glm::vec4(pos, 0.0f);


        float len = glm::length(pos);
        Physics::RaycastPayload payload = Physics::Raycast(position, dir, len);

        if (payload.hit)
        {
            // push_back raycaster ID & hit payload
            collisions.push_back(std::make_tuple(this->colliderID, payload));
        }

    }


    if (collisions.size() > 0)
    {
        ResolveCollisions(collisions);
    }

    // At end of frame, clear collision list
    collisions.clear();

    return;
}

// rotate model to correct position, y? axis * 180?
// check direction rays are cast in - seems to be coming out fo the top

void
PodRacer::ResolveCollisions(std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> &collisions) const
{


    for (const auto &collision : collisions)
    {
        // Reduce/Reverse player speed

        // Reflect players incoming force through the hit object's surface normal?
        // Make player spin?

        // Reduce Score?

        // Remove Health?
        std::cout << "HIT" << std::get<1>(collision).hitPoint.x << std::get<1>(collision).hitPoint.y << std::get<1>(collision).hitPoint.z << std::endl;
    }

    return;

}



}