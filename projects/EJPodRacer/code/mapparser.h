#pragma once
#include "config.h"
#include "gameobject.h"
#include <vector>

// Avoid circular dependency.
class MapChunk;

class MapParser {
public:
    static MapChunk* FromData(const char* binFilePath);
};