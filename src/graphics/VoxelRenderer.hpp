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
    // Async layer generator
    void generateLayer(std::shared_ptr<voxels::Chunk> chunk, uint32_t y);

    // Syncronous push face into the buffer
    inline void pushFace(const Face &face);

    // Make vertex from given values
    inline Vertex mkVtx(float x, float y, float z, float u, float v, float l);

    // Move point to the left
    inline float lftVtx(uint32_t x);

    // Move point to the right
    inline float rgtVtx(uint32_t x);

    // Move point to the up
    inline float upVtx(uint32_t y);

    // Move point to the down
    inline float downVtx(uint32_t y);

    // Move point to the forward
    inline float fwdVtx(uint32_t z);

    // Move point to the backward
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
