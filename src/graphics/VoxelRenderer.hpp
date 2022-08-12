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
    inline void pushVtx(float x, float y, float z, float u, float v, float l);
    inline Vertex makeVtx(float x, float y, float z, float u, float v, float l);
    inline void pushFace(const Face &face);
    void generateLayer(std::shared_ptr<voxels::Chunk> chunk, int32_t y);

    inline float leftVtx(uint32_t x);
    inline float rightVtx(uint32_t x);

    inline float upVtx(uint32_t y);
    inline float downVtx(uint32_t y);

    inline float fwdVtx(uint32_t z);
    inline float bwdVtx(uint32_t z);

private:
    std::vector<float> buffer_;

    static const uint32_t vertexSize_;
    static const int32_t chunkAttributes_[];
    static const uint32_t atlasElement_;

    static const float cubeSideSize_;
    static const float uvsize_;
};

} // namespace graphics
