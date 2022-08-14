#include <voxels/Chunk.hpp>

#include <voxel.hpp>

#include <cmath>

namespace voxels
{

Chunk::Chunk(uint32_t xPos, uint32_t yPos, uint32_t zPos)
    : x_(xPos), y_(yPos), z_(zPos), modified_(true)
{
    for (int32_t y = 0; y < ChunkHeight; ++y)
    {
        for (int32_t z = 0; z < ChunkDepth; ++z)
        {
            for (int32_t x = 0; x < ChunkWidth; ++x)
            {
                uint32_t id = y <= (sin(x * 0.3f) * 0.5f + 0.5f) * 10;

                if (y <= voxels::BlockType::Last)
                {
                    id = voxels::BlockType::Last;
                }

                voxels_[(y * ChunkDepth + z) * ChunkWidth + x].id =
                    static_cast<voxels::BlockType>(id);
            }
        }
    }
}

voxel &voxels::Chunk::operator[](uint32_t idx)
{
    return voxels_[idx];
}

inline bool Chunk::hasNeighbour(uint32_t x, uint32_t y, uint32_t z)
{
    if (x >= voxels::ChunkWidth || y >= voxels::ChunkHeight || z >= voxels::ChunkDepth)
    {
        return false;
    }

    return 0 != GetVoxel(x, y, z).id;
}

voxel &Chunk::GetVoxel(int32_t x, int32_t y, int32_t z)
{
    const uint32_t idx = (y * voxels::ChunkDepth + z) * voxels::ChunkWidth + x;
    return voxels_[idx];
}

int32_t Chunk::GetX()
{
    return x_;
}

int32_t Chunk::GetY()
{
    return y_;
}

int32_t Chunk::GetZ()
{
    return z_;
}

void Chunk::SetModified()
{
    modified_ = true;
}

bool Chunk::RequireUpdate()
{
    return modified_;
}

} // namespace voxels
