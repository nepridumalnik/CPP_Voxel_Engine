#pragma once

#include <graphics/Mesh.hpp>

#include <voxels/Chunk.hpp>

#include <array>
#include <memory>
#include <vector>

namespace graphics
{

class EXPORT VoxelRenderer
{
private:
    using Vertex = std::array<float, 6>;
    using Face = std::array<Vertex, 6>;
    using LightLevel = float;

public:
    std::shared_ptr<Mesh> Render(std::shared_ptr<voxels::Chunk> chunk);

private:
    inline void pushVertex(float x, float y, float z, float u, float v, float l);
    inline Vertex makeVertex(float x, float y, float z, float u, float v, float l);
    inline void pushFace(const Face &face);
    void generateLayer(std::shared_ptr<voxels::Chunk> chunk, int32_t y);

private:
    std::vector<float> buffer_;

    static const uint32_t vertexSize_;
    static const int32_t chunkAttributes_[];
    static const uint32_t atlasElement_;

    static const float uvsize_;
};

} // namespace graphics
