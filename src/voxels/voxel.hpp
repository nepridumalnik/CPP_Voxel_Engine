#pragma once

namespace voxels
{

enum VoxelType
{
    None,
    Grass,
    Dirt
};

struct EXPORT voxel
{
    VoxelType id;
};

} // namespace voxels
