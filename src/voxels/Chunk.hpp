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
    Chunk();

    voxel &operator[](uint32_t idx);

private:
    std::array<voxels::voxel, ChunkVolume> voxels;
};

} // namespace voxels
