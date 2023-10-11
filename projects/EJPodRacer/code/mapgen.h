#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>
#include "podracer.h"
#include "mapchunk.h"




class Mapgen {
public:
    Mapgen();
    ~Mapgen();
    void Init();
    void Generate();
    void Draw();
    void SetPlayer(Game::PodRacer* player);

private:

    std::vector<Render::PointLightId> lights;
    Game::PodRacer* player;
    Render::ModelId sidesModelId;
    Render::ModelId obstaclesModelId;
};
