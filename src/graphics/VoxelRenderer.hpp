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
    VoxelRenderer();

    std::shared_ptr<Mesh> Render(std::shared_ptr<voxels::Chunk> chunk);

private:
    std::vector<float> buffer_;

    static const uint32_t vertexSize_;
    static const uint32_t numberOfSides_;
};

} // namespace graphics
