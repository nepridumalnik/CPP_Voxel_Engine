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
    std::shared_ptr<Mesh> Render(std::shared_ptr<voxels::Chunk> chunk);

private:
    inline void pushVertex(float x, float y, float z, float u, float v, float l);

private:
    std::vector<float> buffer_;
    uint32_t capacity_;

    static const uint32_t vertexSize_;
    static const int32_t chunkAttributes_[];
    static const uint32_t atlasSide_;
};

} // namespace graphics
