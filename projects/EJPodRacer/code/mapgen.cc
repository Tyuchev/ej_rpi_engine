#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"

constexpr float TILE_SIZE = 1.0f;
constexpr int ROAD_WIDTH = 3;
constexpr int GRID_HEIGHT = 10;
constexpr int GRID_WIDTH = 10;

Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racing/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racing/Models/GLTF format/rock_largeA.glb");
}


void Mapgen::GenerateStraightRoad(glm::vec3 topLeftPos) {
    tiles.erase(tiles.begin(), tiles.end());

    const int LEFT = GRID_WIDTH / 2 - ROAD_WIDTH / 2 - 1;
    const int RIGHT = GRID_WIDTH / 2 + ROAD_WIDTH / 2 + 1;

    // Generate sides
    for (int y = 0; y < GRID_HEIGHT; y++) {
        MapTile left;
        glm::vec3 leftPos = glm::vec3(LEFT*TILE_SIZE, y*TILE_SIZE, 0.0f);
        left.model = this->sidesModelId;
        left.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        left.transform = glm::translate(left.transform, leftPos);
        left.transform = left.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
        left.transform = left.transform * glm::rotate(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
        tiles.push_back(left);

        MapTile right;
        glm::vec3 rightPos = glm::vec3(RIGHT*TILE_SIZE, y*TILE_SIZE, 0.0f);
        right.model = this->sidesModelId;
        right.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        right.transform = glm::translate(right.transform, rightPos);
        right.transform = right.transform * glm::rotate(1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));
        right.transform = right.transform * glm::rotate(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
        tiles.push_back(right);
    }

    // Generate road + obstacles
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = LEFT + 1; x < RIGHT; x++) {
            int rowcount = 0;
            // FIXME: Chance is wrong
            if (rowcount <= ROAD_WIDTH - 1 && Core::RandomFloatNTP() < 0.1) {
                MapTile obstacle;
                obstacle.model = this->obstaclesModelId;
                obstacle.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
                obstacle.transform = glm::translate(obstacle.transform, glm::vec3(x*TILE_SIZE, y*TILE_SIZE, 0.0f));
                obstacle.transform = obstacle.transform * glm::rotate(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
                tiles.push_back(obstacle);
                rowcount++;
            }
        }
    }

}

glm::vec3 lastGeneratedPos;
void Mapgen::Generate() {
    float dist = (lastGeneratedPos - player->position).length();
    printf("%f\n", dist);
    if (dist > 1.0f) {
        tiles.erase(tiles.begin(), tiles.end());
        MapTile obstacle;
        obstacle.model = this->obstaclesModelId;
        //obstacle.transform = obstacle.transform * glm::rotate(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
        obstacle.transform = glm::translate(obstacle.transform, player->position);
        obstacle.transform = glm::scale(glm::mat4(1), glm::vec3(10.0f, 10.0f, 10.0f));
        tiles.push_back(obstacle);
        lastGeneratedPos = player->position;
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (const MapTile& tile : tiles) {
        Render::RenderDevice::Draw(tile.model, tile.transform);
    }
}
