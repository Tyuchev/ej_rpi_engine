#pragma once
#include "config.h"
#include "gameobject.h"
#include <vector>

constexpr int CHUNK_WIDTH = 9;
constexpr int CHUNK_LENGTH = 9;
constexpr float TILE_SIZE = 10.0f; 
constexpr float TILE_SCALE = 10.0f;
constexpr int ROAD_WIDTH = 3;
constexpr float TILE_HEIGHT = 0.0f;

enum class Direction {
    North,
    East,
    South,
    West
};

class MapChunk : public GameObject{
public:
    Direction exitDir;
    
    MapChunk();
    void Attach(MapChunk& chunk);
    MapChunk* Duplicate();

private:
    // Assumed to start forward at start, but can change.
    Direction forwardDir = Direction::North;
};