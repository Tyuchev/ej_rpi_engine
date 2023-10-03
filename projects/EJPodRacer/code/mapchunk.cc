#include "config.h"
#include "mapchunk.h"
#include <string>


MapChunk::MapChunk() {
    GameObject::GameObject();
}

MapChunk::MapChunk(const Render::ModelId& chunkModel) {
    model = chunkModel;
}

MapChunk* MapChunk::Duplicate() {
    return nullptr;
}

Direction MapChunk::RotateDirCW(Direction dir, const int& times) {
    Direction returnDir = dir;
    if (times >= 0) {
        for (int i = 0; i < times; i++) {
            switch(returnDir) {
            case Direction::North:
                returnDir = Direction::East;
                continue;
            case Direction::East:
                returnDir = Direction::South;
                continue;
            case Direction::South:
                returnDir = Direction::West;
                continue;
            case Direction::West:
                returnDir = Direction::North;
                continue;
            }
        }
    }
    else {
        for (int i = 0; i < -times; i++) {
            switch(returnDir) {
            case Direction::North:
                returnDir = Direction::West;
                continue;
            case Direction::East:
                returnDir = Direction::North;
                continue;
            case Direction::South:
                returnDir = Direction::East;
                continue;
            case Direction::West:
                returnDir = Direction::South;
                continue;
            }
        }
    }
    return returnDir;
}


void MapChunk::Attach(MapChunk& chunk) {
    glm::vec3 offset;
    constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr float PI = 3.1415927f;
    switch(exitDir) {
    case Direction::North:
        offset = glm::vec3(0.0f, 0.0f, CHUNK_LENGTH * TILE_SCALE);
        break;
    case Direction::East:
        offset = glm::vec3(-CHUNK_WIDTH * TILE_SCALE, 0.0f, 0.0f);
        chunk.Rotate(UP, -PI / 2.0f);
        chunk.forwardDir = RotateDirCW(chunk.forwardDir, 1);
        chunk.exitDir = RotateDirCW(exitDir, 1);
        break;
    case Direction::South:
        offset = glm::vec3(0.0f, 0.0f, -CHUNK_LENGTH * TILE_SCALE);
        chunk.Rotate(UP, PI);
        chunk.forwardDir = RotateDirCW(chunk.forwardDir, 2);
        chunk.exitDir = RotateDirCW(exitDir, 2);
        break;
    case Direction::West:
        offset = glm::vec3(CHUNK_WIDTH * TILE_SCALE, 0.0f, 0.0f);
        chunk.forwardDir = RotateDirCW(chunk.forwardDir, -1);
        chunk.exitDir = RotateDirCW(exitDir, -1);
        break;
    }
    chunk.SetPos(this->GetPos() + offset);
}
