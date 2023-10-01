#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>
#include "debugdraw.h"
#include "mapchunk.h"


Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");

    //this->chunks.push_back(GetStraightRoadChunk());
    //this->chunks.push_back(GameObject());

    printf("Testing flatbuffer:\n\n");
    MapChunk* chunk = MapChunk::FromData("assets/chunk_data/debug_rot_test.bin");
    chunk->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -1.5708f);
    this->chunks.push_back(*chunk);
    printf("Flatbuffer testing complete\n");

}


GameObject Mapgen::GetFilledRoadChunk() {
    GameObject chunk;
    chunk.model = Render::LoadModel("assets/pod_racer/Models/GLTF format/machine_generatorLarge.glb");

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            GameObject* tile = new GameObject();
            glm::vec3 pos = glm::vec3(x*TILE_SIZE, 0.0, z*TILE_SIZE);
            tile->model = this->sidesModelId;
            tile->SetPos(pos);
            tile->Scale(glm::vec3(TILE_SCALE));
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
        GameObject* left = new GameObject();
        glm::vec3 pos = glm::vec3(LEFT*TILE_SIZE, 0.0, z*TILE_SIZE);
        left->model = this->sidesModelId;
        left->SetPos(pos);
        left->Scale(glm::vec3(TILE_SCALE));
        // 1.5708 = pi/2 = 90 deg
        left->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.5708f);
        chunk.AttachChild(left);
    }
    return chunk;
}


std::vector<glm::vec3> GetBroadGeometry(int nBetweenPoints, float widthNoise, float length) {
    std::vector<glm::vec3> points = std::vector<glm::vec3>();
    const float step = length / (nBetweenPoints + 1);
    points.push_back(glm::vec3(0.0f));
    for (float dist = step; dist <= length - step + 0.001f; dist += step) {
        glm::vec3 point;
        point = glm::vec3(Core::RandomFloatNTP() * widthNoise, 0.0f, dist);
        points.push_back(point);
    }
    points.push_back(glm::vec3(0.0f, 0.0f, length));
    return points;
}


// Returns center of chunk closest to point.
glm::vec3 GetClosestChunkPos(glm::vec3 point) {
    glm::vec3 ret = glm::vec3(0.0f);
    ret.x = (int)(point.x / CHUNK_WIDTH) + CHUNK_WIDTH / 2.0f;
    ret.y = 0.0f;
    ret.z = (int)(point.z / CHUNK_LENGTH) + CHUNK_LENGTH / 2.0f;
    return ret;
}

// TODO: New approach. Just generate a chunk and connect others according to presets per chunk piece.
//       Generally moving along the forward axis.



// Called every frame
void Mapgen::Generate() {
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin
    static bool genned = false;
    if (!genned) {
        genned = true;
        //glm::vec3 centerOffset(TILE_SIZE * CHUNK_WIDTH / 2, 0.0f, TILE_SIZE * CHUNK_LENGTH / 2);
        //centerOffset -= glm::vec3(TILE_SIZE / 2, 0.0f, TILE_SIZE / 2);
        //glm::vec3 genPos = -centerOffset;
        //chunks[0].SetPos(genPos);
        for (glm::vec3 point : GetBroadGeometry(2, 10.0f, 30.0f)) {
            DebugDraw::ArrowStraight(point, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const GameObject& chunk : chunks) {
        chunk.Draw();
    }
}

