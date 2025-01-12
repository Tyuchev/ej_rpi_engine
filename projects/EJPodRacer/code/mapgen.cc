#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>
#include "mapparser.h"


// Should be used for fog shader later as well;
constexpr float RENDER_DISTANCE = 200.0f;
constexpr float CHUNK_REMOVAL_DISTANCE = 50.0f;
constexpr float CHUNK_ADD_DISTANCE = 100.0f;

// Debug flags.
constexpr bool MANUAL_CHUNK_DEBUG = false;
constexpr bool ONLY_STRAIGHT_CHUNKS = true;

MapChunk* debugChunk = nullptr;

MapChunkBuilder builder;

bool wasMapInit = false;

Mapgen::Mapgen() {
    this->player = nullptr;
}

// Don't call in constructor.
void Mapgen::Init() {
    builder = MapChunkBuilder();
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    // TODO: Replace by function returning random obstacle model.
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");

    if (MANUAL_CHUNK_DEBUG) {
        //debugChunk = MapParser::FromData("assets/chunk_data/road_straight.bin");
        //debugChunk->SetPos(glm::vec3(-CHUNK_WIDTH/2.0f, 0.0f, -CHUNK_LENGTH/2.0f) * TILE_SCALE);
        builder.AddNext("assets/chunk_models/troad_straight.glb", Direction::North);
        //builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
        //builder.AddNext("assets/chunk_models/road_left.glb", Direction::West);
        //builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    }
    else {
        builder.AddNext("assets/chunk_models/troad_straight.glb", Direction::North);
    }
    wasMapInit = true;
}

void Mapgen::SetPlayer(Game::PodRacer* player) {
    this->player = player;
}

Mapgen::~Mapgen() {
    delete debugChunk;
}

void PlaceNextChunk() {
    float directionChance = Core::RandomFloat();
    constexpr float STRAIGHT_CHANCE = 0.5f;
    // Turn
    if (!ONLY_STRAIGHT_CHUNKS && directionChance > STRAIGHT_CHANCE) {
        const float checkValue = (STRAIGHT_CHANCE + 1) / 2;
        // Left
        if (directionChance < checkValue) {
            builder.AddNext("assets/chunk_models/road_left.glb", Direction::West);
            builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
        }
        // Right
        else if (directionChance >= checkValue) {
            builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
            builder.AddNext("assets/chunk_models/road_left.glb", Direction::West);
        }
    }
    // Straight
    else {
        builder.AddNext("assets/chunk_models/troad_straight.glb", Direction::North);
    }
}


void Mapgen::PlayerDeathFix() {
    if (!hasKilledPlayer)
    {
        player->Kill();
        hasKilledPlayer = true;
    }
}


// Called every frame
void Mapgen::Generate() {
    if (player == nullptr)
        return;
    if (!wasMapInit)
    {
        printf("Error! Mapgen::Init() wasn't called!\n");
        return;
    }
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin

    // Check if we should generate new chunks.
    if (!MANUAL_CHUNK_DEBUG) {
        const MapChunk* lastChunk = builder.GetLastChunk();
        const MapChunk* firstChunk = builder.GetFirstChunk();
        if (lastChunk != nullptr && glm::distance(lastChunk->GetPos(), player->position) <= CHUNK_ADD_DISTANCE) {
            PlaceNextChunk();
        }
        // Check if we should delete past chunks.
        if (firstChunk != nullptr && glm::distance(firstChunk->GetPos(), player->position) >= CHUNK_REMOVAL_DISTANCE) {
            builder.RemoveFirst();
        }


        if (builder.CheckCollision(player->position)) {
            PlayerDeathFix();
        }
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    if (player == nullptr)
        return;
    const std::vector<MapChunk*> chunks = builder.GetChunks();
    for (MapChunk* chunk : chunks) {
        if (chunk == nullptr)
            continue;
        if (MANUAL_CHUNK_DEBUG || glm::distance(chunk->GetPos(), player->position) < RENDER_DISTANCE) {
            chunk->Draw();
        }
    }
    if (debugChunk != nullptr) {
        debugChunk->Draw();
    }
}

