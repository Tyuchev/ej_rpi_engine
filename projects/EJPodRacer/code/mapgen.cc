#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>


Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");

    //this->chunks.push_back(GetStraightRoadChunk());
    this->chunks.push_back(GetFilledRoadChunk());
}


GameObject Mapgen::GetFilledRoadChunk() {
    GameObject chunk;

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            GameObject tile;
            glm::vec3 pos = glm::vec3(x*TILE_SIZE, 0.0, z*TILE_SIZE);
            tile.model = this->sidesModelId;
            tile.SetPos(pos);
            tile.Scale(glm::vec3(TILE_SCALE));
            chunk.AttachChild(tile);
        }
    }
    return chunk;
}

GameObject Mapgen::GetStraightRoadChunk() {
    GameObject chunk;

    // 0 is leftmost tile
    const int RIGHT = 2;
    const int LEFT = 6;

    // Generate sides
    for (int z = 0; z < CHUNK_LENGTH; z++) {
        GameObject left;
        glm::vec3 pos = glm::vec3(LEFT*TILE_SIZE, 0.0, z*TILE_SIZE);
        left.model = this->sidesModelId;
        left.SetPos(pos);
        left.Scale(glm::vec3(TILE_SCALE));
        // 1.5708 = pi/2 = 90 deg
        left.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.5708f);
        chunk.AttachChild(left);
    }


    return chunk;
}


// Called every frame
void Mapgen::Generate() {
    static glm::vec3 lastGeneratedPos = glm::vec3(100.0f, 100.0f, 100.0f);
    glm::vec3 no_y_player_pos = glm::vec3(player->position.x, -1.0f, player->position.z);
    // +x is left of spawn position 
    // +y is up of spawn position
    // +z is forward of spawn position
    glm::vec3 playerOffset(-40.0f, -1.0f, 1.0f); // Centered
    glm::vec3 gen_pos = no_y_player_pos + playerOffset;
    float dist = glm::length(gen_pos - lastGeneratedPos);
    if (dist > 24.0f) {
        // TODO: Actual chunk generation.
        chunks[0].SetPos(gen_pos * 0.1f);
        lastGeneratedPos = gen_pos;
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const GameObject& chunk : chunks) {
        chunk.Draw();
    }
}

