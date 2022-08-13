#include <voxels/Chunks.hpp>

namespace voxels
{

Chunks::Chunks(uint32_t w, uint32_t h, uint32_t d) : w_(w), h_(h), d_(d), volume_(w_ * h_ * d_)
{
    chunks_.resize(volume_);

    for (uint32_t idx = 0, y = 0; y < h_; ++y)
    {
        for (uint32_t z = 0; z < d_; ++z)
        {
            for (uint32_t x = 0; x < w_; ++x)
            {
                std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(x, y, z);
                chunks_[idx++] = chunk;
            }
        }
    }
}

uint32_t Chunks::Size()
{
    return chunks_.size();
}

std::shared_ptr<Chunk> Chunks::At(uint32_t idx)
{
    return chunks_.at(idx);
}

} // namespace voxels
