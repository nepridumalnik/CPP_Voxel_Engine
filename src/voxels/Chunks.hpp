#pragma once

#include <voxels/Chunk.hpp>

#include <memory>
#include <vector>

namespace voxels
{

enum Neighbour : uint8_t
{
    Up = 1,
    Down = 2,
    Left = 3,
    Right = 4,
    Front = 5,
    Back = 6
};

class EXPORT Chunks
{
public:
    Chunks(uint32_t w, uint32_t h, uint32_t d);
    uint32_t Size();
    std::shared_ptr<Chunk> At(uint32_t idx);
    std::shared_ptr<Chunk> GetNeighbour(Neighbour n, uint32_t w, uint32_t h, uint32_t d);

private:
    std::vector<std::shared_ptr<Chunk>> chunks_;

    const uint32_t w_;
    const uint32_t h_;
    const uint32_t d_;
    const uint32_t volume_;
};

} // namespace voxels
