#pragma once

#include <cstdint>

namespace voxels
{

enum BlockType : uint32_t
{
    None,
    Grass,
    Dirt,

    Last = Dirt
};

struct voxel
{
    BlockType id = None;
};

} // namespace voxels
