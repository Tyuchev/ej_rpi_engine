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

    this->chunks.push_back(GetStraightRoadChunk());
}


Chunk Mapgen::GetStraightRoadChunk() {
    Chunk chunk;
    
    const int RIGHT = CHUNK_WIDTH / 2 - ROAD_WIDTH / 2 - 1;
    const int LEFT = CHUNK_WIDTH / 2 + ROAD_WIDTH / 2 + 1;


    // Generate sides
    for (int z = 0; z < CHUNK_LENGTH; z++) {
        MapTile left;
        glm::vec3 leftPos = glm::vec3(LEFT*TILE_SIZE, 0.0f, z*TILE_SIZE);
        left.model = this->sidesModelId;
        left.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        left.transform = glm::translate(left.transform, leftPos);
        // 1.5708 = pi/2 = 90 deg
        left.transform = left.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
        chunk.tiles[z][0] = left;

        MapTile right;
        glm::vec3 rightPos = glm::vec3(RIGHT*TILE_SIZE, 0.0f, z*TILE_SIZE);
        right.model = this->sidesModelId;
        right.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        right.transform = glm::translate(right.transform, rightPos);
        right.transform = right.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
        chunk.tiles[z][CHUNK_WIDTH-1] = right;
    }

    // Generate road + obstacles
    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = RIGHT + 1; x < LEFT; x++) {
            int rowcount = 0;
            if (rowcount <= ROAD_WIDTH - 1 && Core::RandomFloat() < 0.1f) {
                MapTile obstacle;
                obstacle.model = this->obstaclesModelId;
                obstacle.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
                obstacle.transform = glm::translate(obstacle.transform, glm::vec3(x*TILE_SIZE, 0.0f, z*TILE_SIZE));
                obstacle.transform = obstacle.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
                chunk.tiles[z][x] = obstacle;
                rowcount++;
            }
        }
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
    glm::vec3 playerOffset(-52.5f, -1.0f, 1.0f);
    glm::vec3 gen_pos = no_y_player_pos + playerOffset;
    float dist = glm::length(gen_pos - lastGeneratedPos);
    if (dist > 24.0f) {
        chunks[0].position = gen_pos;
        lastGeneratedPos = gen_pos;
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const Chunk& chunk : chunks) {
        chunk.Draw();
    }
}

void Chunk::Draw() const {
    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            glm::mat4 rel_transform = glm::translate(tiles[z][x].transform, position);
            Render::RenderDevice::Draw(tiles[z][x].model, rel_transform);
        }
    }
}

