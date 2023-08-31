#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>

constexpr float TILE_SIZE = 1.0f;
constexpr int ROAD_WIDTH = 3;
constexpr int GRID_HEIGHT = 10;
constexpr int GRID_WIDTH = 10;

Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");
}


void Mapgen::GenerateStraightRoad(glm::vec3 topLeftPos) {
    tiles.erase(tiles.begin(), tiles.end());

    const int RIGHT = GRID_WIDTH / 2 - ROAD_WIDTH / 2 - 1;
    const int LEFT = GRID_WIDTH / 2 + ROAD_WIDTH / 2 + 1;

    // +x is left of spawn position 
    // +y is up of spawn position
    // +z is forward of spawn position

    // Generate sides
    for (int y = 0; y < GRID_HEIGHT; y++) {
        MapTile left;
        glm::vec3 leftPos = glm::vec3(LEFT*TILE_SIZE, 0.0f, y*TILE_SIZE) + (topLeftPos * 0.1f);
        left.model = this->sidesModelId;
        left.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        left.transform = glm::translate(left.transform, leftPos);
        // 1.5708 = pi/2 = 90 deg
        left.transform = left.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
        tiles.push_back(left);

        MapTile right;
        glm::vec3 rightPos = glm::vec3(RIGHT*TILE_SIZE, 0.0f, y*TILE_SIZE) + (topLeftPos * 0.1f);
        right.model = this->sidesModelId;
        right.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        right.transform = glm::translate(right.transform, rightPos);
        right.transform = right.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
        tiles.push_back(right);
    }

    // Generate road + obstacles
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = RIGHT + 1; x < LEFT; x++) {
            int rowcount = 0;
            if (rowcount <= ROAD_WIDTH - 1 && Core::RandomFloat() < 0.1f) {
                MapTile obstacle;
                obstacle.model = this->obstaclesModelId;
                obstacle.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
                obstacle.transform = glm::translate(obstacle.transform, glm::vec3(x*TILE_SIZE, 0.0f, y*TILE_SIZE) + (topLeftPos * 0.1f));
                obstacle.transform = obstacle.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
                tiles.push_back(obstacle);
                rowcount++;
            }
        }
    }

}

// Called every frame
void Mapgen::Generate() {
    static glm::vec3 lastGeneratedPos = glm::vec3(100.0f, 100.0f, 100.0f);
    glm::vec3 no_y_player_pos = glm::vec3(player->position.x, -1.0f, player->position.z);
    glm::vec3 playerOffset(-52.5f, -1.0f, 1.0f);
    glm::vec3 gen_pos = no_y_player_pos + playerOffset;
    float dist = glm::length(gen_pos - lastGeneratedPos);
    if (dist > 24.0f) {
        GenerateStraightRoad(gen_pos);
        lastGeneratedPos = gen_pos;
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const MapTile& tile : tiles) {
        Render::RenderDevice::Draw(tile.model, tile.transform);
    }
}
