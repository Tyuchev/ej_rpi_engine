#include "config.h"
#include "mapchunk.h"
#include <string>
#include "core/random.h"


MapChunk::MapChunk(const Render::ModelId& chunkModel) {
    model = chunkModel;
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


MapChunkBuilder::MapChunkBuilder() {
    lastChunk = nullptr;
    firstChunk = nullptr;
    chunks = std::vector<MapChunk*>();
}

MapChunkBuilder::~MapChunkBuilder() {
    for (MapChunk* chunk : chunks) {
        delete chunk;
    }
    chunks.clear();
}


void MapChunk::Attach(MapChunk* chunk) {
    glm::vec3 offset;
    constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr float PI = 3.1415927f;
    switch(exitDir) {
    case Direction::North:
        offset = glm::vec3(0.0f, 0.0f, CHUNK_LENGTH * TILE_SCALE);
        break;
    case Direction::East:
        offset = glm::vec3(-CHUNK_WIDTH * TILE_SCALE, 0.0f, 0.0f);
        chunk->Rotate(UP, -PI / 2.0f);
        chunk->forwardDir = exitDir;
        chunk->exitDir = RotateDirCW(chunk->exitDir, 1);
        break;
    case Direction::South:
        offset = glm::vec3(0.0f, 0.0f, -CHUNK_LENGTH * TILE_SCALE);
        chunk->Rotate(UP, PI);
        chunk->forwardDir = exitDir;
        chunk->exitDir = RotateDirCW(chunk->exitDir, 2);
        break;
    case Direction::West:
        offset = glm::vec3(CHUNK_WIDTH * TILE_SCALE, 0.0f, 0.0f);
        chunk->Rotate(UP, PI / 2.0f);
        chunk->forwardDir = exitDir;
        chunk->exitDir = RotateDirCW(chunk->exitDir, -1);
        break;
    }
    chunk->SetPos(this->GetPos() + offset);
}

GameObject* GetObstacleTile() {
    GameObject* tile = new GameObject();
    tile->model = Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");
    tile->Scale(glm::vec3(TILE_SCALE));
    return tile;
}

void AddObstaclesToChunk(MapChunk* chunk) {
    const int RIGHT = (CHUNK_WIDTH - ROAD_WIDTH) / 2 + 1;
    const int LEFT = CHUNK_WIDTH - RIGHT + 1;
    const glm::vec3 CENTER_OFFSET = glm::vec3(
        TILE_SIZE * CHUNK_WIDTH / 2,
        0.0,
        -TILE_SIZE * CHUNK_LENGTH / 2);

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = RIGHT; x < LEFT; x++) {
            if (Core::RandomFloat() < OBSTACLE_SPAWN_CHANCE) {
                GameObject* tile = GetObstacleTile();
                tile->isRoad = chunk->isRoad;
                chunk->AttachChild(tile);
                tile->SetPos(CENTER_OFFSET + glm::vec3(
                    -x * TILE_SIZE,
                    0.0f,
                    z * TILE_SIZE));
            }
        }
    }
}

void MapChunkBuilder::AddNext(const char* model, const Direction& exitDir) {
    MapChunk* chunk = new MapChunk(Render::LoadModel(model));
    chunk->isRoad = true;
    chunk->exitDir = exitDir;
    chunk->Scale(glm::vec3(TILE_SCALE));
    if (lastChunk != nullptr) {
        lastChunk->Attach(chunk);
    }
    lastChunk = chunk;
    if (firstChunk == nullptr) {
        firstChunk = chunk;
    }
    AddObstaclesToChunk(chunk);
    chunks.push_back(chunk);
}

bool MapChunkBuilder::CheckCollision(const glm::vec3& position) {
    //const glm::vec3 CENTER_OFFSET = glm::vec3(-TILE_SIZE / 2.0f, 0.0, -TILE_SIZE / 2.0f);
    const glm::vec3 CENTER_OFFSET = glm::vec3(0.0f);
    float absX = glm::abs(position.x);
    printf("%f\n", abs(absX));
    // Temporary fix.
    if (absX > 18.0f) {
        return true;
    }
    for (auto chunk : chunks) {
        if (chunk != nullptr) {
            for (auto obstacle : chunk->children) {
                if (glm::distance(obstacle->GetWorldPos() + CENTER_OFFSET, position) < TILE_SIZE / 2.0f)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

const std::vector<MapChunk*>& MapChunkBuilder::GetChunks() const {
    return chunks;
}

const MapChunk* MapChunkBuilder::GetLastChunk() const {
    return lastChunk;
}

const MapChunk* MapChunkBuilder::GetFirstChunk() const {
    return firstChunk;
}

void MapChunkBuilder::RemoveFirst() {
    chunks.erase(chunks.begin());
    delete firstChunk;
    firstChunk = chunks[0];
}