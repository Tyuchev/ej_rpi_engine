#pragma once
#include "config.h"
#include "render/model.h"
#include "render/lightserver.h"
#include <vector>
#include "podracer.h"


struct MapTile {
    // Could have made and inherited from a base object class but should be fine for now.
    // (since it shares common functionality with Spaceship)
    glm::vec3 position = glm::vec3(0);
    glm::quat orientation = glm::identity<glm::quat>();
    glm::mat4 transform = glm::mat4(1);
    Render::ModelId model;
};

class Mapgen {
public:
    Mapgen(Game::PodRacer* const player);
    void Generate();
    void Draw();

private:
    void GenerateStraightRoad(glm::vec3 topLeftPos);
    std::vector<MapTile> tiles;
    std::vector<Render::PointLightId> lights;
    Game::PodRacer* const player;
    Render::ModelId sidesModelId;
    Render::ModelId obstaclesModelId;

};
