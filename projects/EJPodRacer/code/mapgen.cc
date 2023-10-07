#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>

// Should be used for fog shader later as well;
constexpr float RENDER_DISTANCE = 100.0f;
constexpr float CHUNK_REMOVAL_DISTANCE = 200.0f;
constexpr float CHUNK_ADD_DISTANCE = 100.0f;

MapChunkBuilder builder = MapChunkBuilder();

Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");

    builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    //builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
    //builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
    //builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    //builder.AddNext("assets/chunk_models/road_left.glb", Direction::West);
    //builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
}


// Called every frame
void Mapgen::Generate() {
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin

    // Check if we should generate new chunks.
    if (glm::distance(builder.GetLastChunk()->GetPos(), player->position) <= CHUNK_ADD_DISTANCE) {
        builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    }
    // Check if we should delete past chunks.
    if (glm::distance(builder.GetFirstChunk()->GetPos(), player->position) >= CHUNK_REMOVAL_DISTANCE) {
        builder.RemoveFirst();
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    const std::vector<MapChunk*> chunks = builder.GetChunks();
    for (MapChunk* chunk : chunks) {
        if (glm::distance(chunk->GetPos(), player->position) < RENDER_DISTANCE) {
            chunk->Draw();
        }
    }
}

