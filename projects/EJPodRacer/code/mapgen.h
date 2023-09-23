#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>
#include "podracer.h"
#include "gameobject.h"

constexpr int CHUNK_WIDTH = 9;
constexpr int CHUNK_LENGTH = 9;
constexpr float TILE_SIZE = 10.0f; 
constexpr float TILE_SCALE = 10.0f;
constexpr int ROAD_WIDTH = 3;



class Mapgen {
public:
    Mapgen(Game::PodRacer* const player);
    void Generate();
    void Draw();

private:
    GameObject GetStraightRoadChunk();
    GameObject GetFilledRoadChunk();

    std::vector<GameObject> chunks;
    std::vector<Render::PointLightId> lights;
    Game::PodRacer* const player;
    Render::ModelId sidesModelId;
    Render::ModelId obstaclesModelId;
};
