#pragma once
#include "config.h"
#include "render/model.h"
#include <vector>


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
    Mapgen();
    void Generate();
    void Draw();

private:
    std::vector<MapTile> tiles;
    Render::ModelId sidesModelId;
    Render::ModelId obstaclesModelId;

};
