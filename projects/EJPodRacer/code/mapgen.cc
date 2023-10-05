#include "config.h"
#include "mapgen.h"
#include "podracer.h"
#include "render/renderdevice.h"
#include <mat4x4.hpp> // glm::mat4
#include "gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "gtc/quaternion.hpp"
#include "core/random.h"
#include <iostream>
#include "debugdraw.h"


MapChunkBuilder builder = MapChunkBuilder();

Mapgen::Mapgen(Game::PodRacer* const player) : player(player) {
    this->sidesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rail.glb");
    this->obstaclesModelId = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");

    builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
    builder.AddNext("assets/chunk_models/road_right.glb", Direction::East);
    builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);
    builder.AddNext("assets/chunk_models/road_left.glb", Direction::West);
    builder.AddNext("assets/chunk_models/road_straight.glb", Direction::North);

    // TODO: If track is supposed to be infinite, then this needs to change.
    chunks = builder.GetChunks();
}

// TODO: If unused delete.
std::vector<glm::vec3> GetBroadGeometry(int nBetweenPoints, float widthNoise, float length) {
    std::vector<glm::vec3> points = std::vector<glm::vec3>();
    const float step = length / (nBetweenPoints + 1);
    points.push_back(glm::vec3(0.0f));
    for (float dist = step; dist <= length - step + 0.001f; dist += step) {
        glm::vec3 point;
        point = glm::vec3(Core::RandomFloatNTP() * widthNoise, 0.0f, dist);
        points.push_back(point);
    }
    points.push_back(glm::vec3(0.0f, 0.0f, length));
    return points;
}


// TODO: If unused delete.
// Returns center of chunk closest to point.
glm::vec3 GetClosestChunkPos(glm::vec3 point) {
    glm::vec3 ret = glm::vec3(0.0f);
    ret.x = (int)(point.x / CHUNK_WIDTH) + CHUNK_WIDTH / 2.0f;
    ret.y = 0.0f;
    ret.z = (int)(point.z / CHUNK_LENGTH) + CHUNK_LENGTH / 2.0f;
    return ret;
}

// TODO: New approach. Just generate a chunk and connect others according to presets per chunk piece.
//       Generally moving along the forward axis.



// Called every frame
void Mapgen::Generate() {
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin
    static bool genned = false;
    if (!genned) {
        genned = true;
        //glm::vec3 centerOffset(TILE_SIZE * CHUNK_WIDTH / 2, 0.0f, TILE_SIZE * CHUNK_LENGTH / 2);
        //centerOffset -= glm::vec3(TILE_SIZE / 2, 0.0f, TILE_SIZE / 2);
        //glm::vec3 genPos = -centerOffset;
        //chunks[0].SetPos(genPos);
        for (glm::vec3 point : GetBroadGeometry(2, 10.0f, 30.0f)) {
            DebugDraw::ArrowStraight(point, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
}

// Call in render loop before RenderDevice::Render()
void Mapgen::Draw() {
    for (auto chunk : chunks) {
        chunk->Draw();
    }
}

