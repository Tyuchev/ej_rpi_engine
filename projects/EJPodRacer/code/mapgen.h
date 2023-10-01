#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>
#include "podracer.h"
#include "gameobject.h"




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
