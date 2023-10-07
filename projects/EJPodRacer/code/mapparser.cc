#include "config.h"
#include "mapparser.h"
#include <fstream>
#include "mapchunk.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"
#include "chunkdata_generated.h"

constexpr const char *SCHEMA_PATH = "chunk_data/chunkdata.fbs";

void PrettyPrintData(const GameData::ChunkData * data) {
    printf("Name: %s\n", data->name()->c_str());
    printf("\tExit dir: %s\n", data->exit_direction()->c_str());
    printf("\tTile map:\n");
    for (int i = 0; i < data->tile_map()->size(); i++) {
        printf("\t\t%s\n", data->tile_map()->Get(i)->c_str());
    }
    printf("\tRotation map:\n");
    for (int i = 0; i < data->rotation_map()->size(); i++) {
        printf("\t\t%s\n", data->rotation_map()->Get(i)->c_str());
    }
    printf("\tMeta map:\n");
    for (int i = 0; i < data->meta_map()->size(); i++) {
        printf("\t\t%s\n", data->meta_map()->Get(i)->c_str());
    }
}

Render::ModelId GetModel(const char& tileKey) {
    switch (tileKey) {
    case 'R':
        return Render::LoadModel("assets/pod_racer/Models/GLTF format/rock_largeA.glb");
    case 'r':
        return Render::LoadModel("assets/pod_racer/Models/GLTF format/rail_end.glb");
    case '|':
        return Render::LoadModel("assets/pod_racer/Models/GLTF format/rail_middle.glb");
    case 'S':
        return Render::LoadModel("assets/pod_racer/Models/GLTF format/stairs.glb");
    case ' ':
        return INVALID_MODEL_ID;
    default:
        printf("Mapchunk: Tile map key not linked to model.\n");
        return INVALID_MODEL_ID;
    }
}

void ApplyRotationToTile(GameObject* tile, const char& rotKey) {
    constexpr glm::vec3 UP(0.0f, 1.0f, 0.0f);
    constexpr float PI = 3.1415927f;
    switch(rotKey) {
    case 'N':
    case 'n':
        return;
    case 'E':
    case 'e':
        tile->Rotate(UP, -PI/2.0f);
        return;
    case 'S':
    case 's':
        tile->Rotate(UP, PI);
        return;
    case 'W':
    case 'w':
        tile->Rotate(UP, PI/2.0f);
        return;
    default:
        printf("Mapchunk: Tile rotation key invalid.\n");
    }
}

void ApplyMetaToTile(GameObject* tile, const char& metaKey, const int& gridX, const int& gridZ) {
    // Minus because we want top left of data to correspond to top left of chunk.
    glm::vec3 position = glm::vec3((CHUNK_WIDTH - gridX) * TILE_SIZE, TILE_HEIGHT, (CHUNK_LENGTH - gridZ) * TILE_SIZE);
    tile->SetPos(position);
    tile->Scale(glm::vec3(TILE_SCALE));
    switch(metaKey) {
    case ' ':
        return;
    case 'N':
        tile->Translate(glm::vec3(0.0f, 0.0f, 1.0f) * TILE_SIZE);
        return;
    case 'E':
        tile->Translate(glm::vec3(-1.0f, 0.0f, 0.0f) * TILE_SIZE);
        return;
    case 'S':
        tile->Translate(glm::vec3(0.0f, 0.0f, -1.0f) * TILE_SIZE);
        return;
    case 'W':
        tile->Translate(glm::vec3(1.0f, 0.0f, 0.0f) * TILE_SIZE);
        return;
    default:
        printf("Mapchunk: Tile meta key invalid.\n");
    }
}

GameObject* GetTile(const char& tileKey, const char& rotKey, const char& metaKey, const int& gridX, const int& gridZ) {
    GameObject* tile = new GameObject();
    tile->isRoad = true;
    // TODO: Might not want to load a game object if key is " ".
    tile->model = GetModel(tileKey);
    ApplyMetaToTile(tile, metaKey, gridX, gridZ);
    ApplyRotationToTile(tile, rotKey);
    return tile;
}

bool IsDataValid(const GameData::ChunkData* data) {
    // Test accessing stuff.
    const flatbuffers::String* name = data->name();
    const flatbuffers::String* exitDirection = data->exit_direction();
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* tileMap = data->tile_map();
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* rotMap = data->rotation_map();
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* metaMap = data->meta_map();

    // Keys are validated in their respective functions. Could
    // do it here for consistency but eh.
    bool ok = true;
    if (tileMap->size() != CHUNK_LENGTH) {
        printf("Mapchunk: Tile map length does not match chunk length.\n");
        bool ok = false;
    }
    if (tileMap->Get(0)->str().length() != CHUNK_WIDTH) {
        printf("Mapchunk: Tile map width does not match chunk width.\n");
        bool ok = false;
    }
    if (rotMap->size() != CHUNK_LENGTH) {
        printf("Mapchunk: Rotation map length does not match chunk length.\n");
        bool ok = false;
    }
    if (rotMap->Get(0)->str().length() != CHUNK_WIDTH) {
        printf("Mapchunk: Rotation map width does not match chunk width.\n");
        bool ok = false;
    }
    if (metaMap->size() != CHUNK_LENGTH) {
        printf("Mapchunk: Meta map length does not match chunk length.\n");
        bool ok = false;
    }
    if (metaMap->Get(0)->str().length() != CHUNK_WIDTH) {
        printf("Mapchunk: Meta map width does not match chunk width.\n");
        bool ok = false;
    }
    return ok;
}

bool ParseData(const GameData::ChunkData* data, MapChunk* outChunk) {
    if (!IsDataValid(data)) {
        printf("Mapchunk: Parsing data failed, data not valid.\n");
        return false;
    }
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* tileMap = data->tile_map();
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* rotMap = data->rotation_map();
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* metaMap = data->meta_map();

    switch(data->exit_direction()->str().at(0)) {
    case 'N':
    case 'n':
        outChunk->exitDir = Direction::North;
        break;
    case 'E':
    case 'e':
        outChunk->exitDir = Direction::East;
        break;
    case 'S':
    case 's':
        outChunk->exitDir = Direction::South;
        break;
    case 'W':
    case 'w':
        outChunk->exitDir = Direction::West;
        break;
    default:
        printf("Mapchunk: Exit direction key invalid.\n");
        outChunk->exitDir = Direction::North;
    }

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            GameObject* tile = GetTile(
                tileMap->Get(z)->str().at(x),
                rotMap->Get(z)->str().at(x),
                metaMap->Get(z)->str().at(x),
                x,
                z);
            outChunk->AttachChild(tile);
        }
    }
    outChunk->isRoad = true;
    return true;
}

MapChunk* MapParser::FromData(const char* binFilePath) {
    char* inputBuffer = new char[4096]('\0');
    std::ifstream fin(binFilePath, std::ios::in | std::ios::binary);
    int charCount = 0;
    while (fin) {
        fin.read(inputBuffer, 1024);
        charCount += fin.gcount();
    }
    // TODO: Check memory leak.
    const GameData::ChunkData* data = GameData::GetChunkData(inputBuffer);
    MapChunk* chunk = new MapChunk();
    bool ok = ParseData(data, chunk);

    if (ok) {
        return chunk;
    }
    return nullptr;
}