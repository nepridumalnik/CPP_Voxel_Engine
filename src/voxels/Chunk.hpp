#pragma once

#include <voxels/voxel.hpp>

#include <array>

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

    std::array<std::array<voxels::voxel, ChunkWidth>, ChunkHeight> &operator[](uint32_t idx);

private:
    std::array<std::array<std::array<voxel, ChunkWidth>, ChunkHeight>, ChunkDepth> voxels_;
};

} // namespace voxels
