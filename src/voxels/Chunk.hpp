#pragma once

#include <voxels/voxel.hpp>

#include <array>
#include <cstdint>

namespace voxels
{

static constexpr uint32_t ChunkWidth = 16;
static constexpr uint32_t ChunkHeight = 16;
static constexpr uint32_t ChunkDepth = 16;
static constexpr uint32_t ChunkVolume = ChunkWidth * ChunkHeight * ChunkDepth;

class EXPORT Chunk
{
public:
    Chunk(uint32_t xPos, uint32_t yPos, uint32_t zPos);

    voxel &operator[](uint32_t idx);
    inline bool hasNeighbour(uint32_t x, uint32_t y, uint32_t z);
    voxel &GetVoxel(int32_t x, int32_t y, int32_t z);
    void SetModified(bool state);

    int32_t GetX();
    int32_t GetY();
    int32_t GetZ();

private:
    std::array<voxels::voxel, ChunkVolume> voxels_;
    bool modified_;

    const int32_t x_;
    const int32_t y_;
    const int32_t z_;
};

} // namespace voxels
