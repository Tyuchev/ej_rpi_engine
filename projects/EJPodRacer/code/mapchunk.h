#pragma once
#include "config.h"
#include "gameobject.h"
#include <vector>

constexpr int CHUNK_WIDTH = 9;
constexpr int CHUNK_LENGTH = 9;
constexpr float TILE_SIZE = 10.0f; 
constexpr float TILE_SCALE = 10.0f;
constexpr float TILE_HEIGHT = 0.0f;

enum class Direction {
    North,
    East,
    South,
    West
};

class MapChunk : public GameObject {
public:
    Direction exitDir;
    // Assumed to start forward at start, but can change.
    Direction forwardDir = Direction::North;
    
    MapChunk() = default;
    MapChunk(const Render::ModelId& chunkModel);
    void Attach(MapChunk* chunk);

private:

    Direction RotateDirCW(Direction dir, const int& times);
};

class MapChunkBuilder {
public:
    MapChunkBuilder();
    ~MapChunkBuilder();

    void AddNext(const char* model, const Direction& exitDir);
    const MapChunk* GetLastChunk() const;
    const MapChunk* GetFirstChunk() const;
    void RemoveFirst();
    const std::vector<MapChunk*>& GetChunks() const;

private:
    MapChunk* lastChunk;
    MapChunk* firstChunk;
    std::vector<MapChunk*> chunks;
};