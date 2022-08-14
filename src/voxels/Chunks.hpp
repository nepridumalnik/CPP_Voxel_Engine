#pragma once

#include <voxels/Chunk.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace voxels
{

class EXPORT Chunks
{
    template <class T> using NullableReference = std::optional<std::reference_wrapper<T>>;

public:
    Chunks(uint32_t w, uint32_t h, uint32_t d);
    uint32_t Size();
    std::shared_ptr<Chunk> At(uint32_t idx);
    std::shared_ptr<Chunk> GetChunkByChunksPos(uint32_t x, uint32_t y, uint32_t z);
    std::shared_ptr<Chunk> GetChunkByCoord(uint32_t x, uint32_t y, uint32_t z);
    NullableReference<voxels::voxel> GetVoxelByCoord(uint32_t x, uint32_t y, uint32_t z);
    NullableReference<voxels::voxel> RayCast(glm::vec3 a, glm::vec3 dir, float maxDist,
                                             glm::vec3 &end, glm::vec3 &norm, glm::vec3 &iend);

    // АЛгоритм целиком спизжен, не понимаю что происходит
    void SetVoxelByCoord(uint32_t x, uint32_t y, uint32_t z, BlockType type);

private:
    std::vector<std::shared_ptr<Chunk>> chunks_;

    const uint32_t w_;
    const uint32_t h_;
    const uint32_t d_;
    const uint32_t volume_;
};

} // namespace voxels
