// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CHUNKDATA_GAMEDATA_H_
#define FLATBUFFERS_GENERATED_CHUNKDATA_GAMEDATA_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 3,
             "Non-compatible flatbuffers version included");

namespace GameData {

struct ChunkData;
struct ChunkDataBuilder;

struct ChunkData FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ChunkDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_EXIT_DIRECTION = 6,
    VT_TILE_MAP = 8,
    VT_ROTATION_MAP = 10,
    VT_META_MAP = 12
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  const ::flatbuffers::String *exit_direction() const {
    return GetPointer<const ::flatbuffers::String *>(VT_EXIT_DIRECTION);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *tile_map() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_TILE_MAP);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *rotation_map() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_ROTATION_MAP);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *meta_map() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_META_MAP);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_EXIT_DIRECTION) &&
           verifier.VerifyString(exit_direction()) &&
           VerifyOffset(verifier, VT_TILE_MAP) &&
           verifier.VerifyVector(tile_map()) &&
           verifier.VerifyVectorOfStrings(tile_map()) &&
           VerifyOffset(verifier, VT_ROTATION_MAP) &&
           verifier.VerifyVector(rotation_map()) &&
           verifier.VerifyVectorOfStrings(rotation_map()) &&
           VerifyOffset(verifier, VT_META_MAP) &&
           verifier.VerifyVector(meta_map()) &&
           verifier.VerifyVectorOfStrings(meta_map()) &&
           verifier.EndTable();
  }
};

struct ChunkDataBuilder {
  typedef ChunkData Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(ChunkData::VT_NAME, name);
  }
  void add_exit_direction(::flatbuffers::Offset<::flatbuffers::String> exit_direction) {
    fbb_.AddOffset(ChunkData::VT_EXIT_DIRECTION, exit_direction);
  }
  void add_tile_map(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> tile_map) {
    fbb_.AddOffset(ChunkData::VT_TILE_MAP, tile_map);
  }
  void add_rotation_map(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> rotation_map) {
    fbb_.AddOffset(ChunkData::VT_ROTATION_MAP, rotation_map);
  }
  void add_meta_map(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> meta_map) {
    fbb_.AddOffset(ChunkData::VT_META_MAP, meta_map);
  }
  explicit ChunkDataBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ChunkData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ChunkData>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ChunkData> CreateChunkData(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    ::flatbuffers::Offset<::flatbuffers::String> exit_direction = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> tile_map = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> rotation_map = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> meta_map = 0) {
  ChunkDataBuilder builder_(_fbb);
  builder_.add_meta_map(meta_map);
  builder_.add_rotation_map(rotation_map);
  builder_.add_tile_map(tile_map);
  builder_.add_exit_direction(exit_direction);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<ChunkData> CreateChunkDataDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *exit_direction = nullptr,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *tile_map = nullptr,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *rotation_map = nullptr,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *meta_map = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto exit_direction__ = exit_direction ? _fbb.CreateString(exit_direction) : 0;
  auto tile_map__ = tile_map ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*tile_map) : 0;
  auto rotation_map__ = rotation_map ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*rotation_map) : 0;
  auto meta_map__ = meta_map ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*meta_map) : 0;
  return GameData::CreateChunkData(
      _fbb,
      name__,
      exit_direction__,
      tile_map__,
      rotation_map__,
      meta_map__);
}

inline const GameData::ChunkData *GetChunkData(const void *buf) {
  return ::flatbuffers::GetRoot<GameData::ChunkData>(buf);
}

inline const GameData::ChunkData *GetSizePrefixedChunkData(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<GameData::ChunkData>(buf);
}

inline bool VerifyChunkDataBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<GameData::ChunkData>(nullptr);
}

inline bool VerifySizePrefixedChunkDataBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<GameData::ChunkData>(nullptr);
}

inline void FinishChunkDataBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<GameData::ChunkData> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedChunkDataBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<GameData::ChunkData> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace GameData

#endif  // FLATBUFFERS_GENERATED_CHUNKDATA_GAMEDATA_H_
