#pragma once

#include <graphics/Mesh.hpp>

#include <voxels/Chunk.hpp>

#include <memory>
#include <vector>

namespace graphics
{

class EXPORT VoxelRenderer
{
public:
    VoxelRenderer(uint32_t capacity);

    std::shared_ptr<Mesh> Render(std::shared_ptr<voxels::Chunk> chunk);

private:
    inline void pushVertex(float x, float y, float z, float u, float v, float l);
    inline bool hasNeighbour(std::shared_ptr<voxels::Chunk> chunk, uint32_t x, uint32_t y,
                             uint32_t z);

private:
    std::vector<float> buffer_;
    uint32_t capacity_;

    static const uint32_t vertexSize_;
    static const int32_t chunkAttributes_[];
};

} // namespace graphics
