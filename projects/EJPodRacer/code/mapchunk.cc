#include "config.h"
#include "mapchunk.h"
#include <string>



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
        chunk->Rotate(UP, -PI / 2.0f);
        chunk->forwardDir = exitDir;
        chunk->exitDir = RotateDirCW(chunk->exitDir, -1);
        break;
    }
    chunk->SetPos(this->GetPos() + offset);
}

void MapChunkBuilder::AddNext(const char* model, const Direction& exitDir) {
    MapChunk* chunk = new MapChunk(Render::LoadModel(model));
    chunk->exitDir = exitDir;
    chunk->Scale(glm::vec3(TILE_SCALE));
    if (lastChunk != nullptr) {
        lastChunk->Attach(chunk);
    }
    lastChunk = chunk;
    if (firstChunk == nullptr) {
        firstChunk = chunk;
    }
    chunks.push_back(chunk);
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
    // TODO: See if you need to delete firstChunk here.
    firstChunk = chunks[0];
}