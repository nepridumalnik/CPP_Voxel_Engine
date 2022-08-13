#pragma once

#include <voxels/Chunk.hpp>

#include <memory>
#include <vector>

namespace voxels
{

class EXPORT Chunks
{
public:
    Chunks(uint32_t w, uint32_t h, uint32_t d);
    uint32_t Size();
    std::shared_ptr<Chunk> At(uint32_t idx);

private:
    std::vector<std::shared_ptr<Chunk>> chunks_;

    const uint32_t w_;
    const uint32_t h_;
    const uint32_t d_;
    const uint32_t volume_;
};

} // namespace voxels
