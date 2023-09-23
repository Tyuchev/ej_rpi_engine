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
    chunk.model = Render::LoadModel("assets/pod_racer/Models/GLTF format/machine_generatorLarge.glb");

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
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin
    static bool genned = false;
    if (!genned) {
        genned = true;
        glm::vec3 centerOffset(TILE_SIZE * CHUNK_WIDTH / 2, 0.0f, TILE_SIZE * CHUNK_LENGTH / 2);
        centerOffset -= glm::vec3(TILE_SIZE / 2, 0.0f, TILE_SIZE / 2);
        glm::vec3 genPos = -centerOffset;
        chunks[0].SetPos(genPos);
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const GameObject& chunk : chunks) {
        chunk.Draw();
    }
}

