#include <voxels/Chunks.hpp>

#include <future>
#include <vector>

namespace voxels
{

Chunks::Chunks(uint32_t w, uint32_t h, uint32_t d) : w_(w), h_(h), d_(d), volume_(w_ * h_ * d_)
{
    std::vector<std::future<std::shared_ptr<Chunk>>> futures;

    futures.resize(volume_);
    chunks_.resize(volume_);

    for (uint32_t idx = 0, y = 0; y < h_; ++y)
    {
        for (uint32_t z = 0; z < d_; ++z)
        {
            for (uint32_t x = 0; x < w_; ++x)
            {
                static const auto lambda = [](uint32_t x, uint32_t y,
                                              uint32_t z) -> std::shared_ptr<Chunk> {
                    return std::make_shared<Chunk>(x, y, z);
                };
                futures[idx++] = std::async(std::launch::async, lambda, x, y, z);
            }
        }
    }

    for (uint32_t i = 0; i < futures.size(); ++i)
    {
        chunks_[i] = futures[i].get();
    }
}

uint32_t Chunks::Size()
{
    return chunks_.size();
}

std::shared_ptr<Chunk> Chunks::At(uint32_t idx)
{
    return chunks_[idx];
}

} // namespace voxels
