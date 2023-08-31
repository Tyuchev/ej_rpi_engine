#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>
#include "podracer.h"

constexpr int CHUNK_WIDTH = 9;
constexpr int CHUNK_LENGTH = 9;
constexpr float TILE_SIZE = 1.0f; 
constexpr int ROAD_WIDTH = 3;

struct MapTile {
    // Could have made and inherited from a base object class but should be fine for now.
    // (since it shares common functionality with Spaceship)
    glm::vec3 position = glm::vec3(0);
    glm::quat orientation = glm::identity<glm::quat>();
    glm::mat4 transform = glm::mat4(1);
    Render::ModelId model;
};

class Chunk {
public:
    glm::vec3 position = glm::vec3(0);
    MapTile tiles[CHUNK_LENGTH][CHUNK_WIDTH];

    void Draw() const;
};

class Mapgen {
public:
    Mapgen(Game::PodRacer* const player);
    void Generate();
    void Draw();

private:
    Chunk GetStraightRoadChunk();

    std::vector<Chunk> chunks;
    std::vector<Render::PointLightId> lights;
    Game::PodRacer* const player;
    Render::ModelId sidesModelId;
    Render::ModelId obstaclesModelId;

};
