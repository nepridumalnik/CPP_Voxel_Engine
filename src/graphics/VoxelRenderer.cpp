#include <graphics/VoxelRenderer.hpp>

int chunk_attrs[] = {3, 2, 1, 0};

static constexpr uint32_t VertexSize = 3 + 2 + 1;
static constexpr int32_t ChunkAttributes[] = {3, 2, 1, 0};

namespace graphics
{

const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;
const uint32_t VoxelRenderer::numberOfSides_ = 6;

VoxelRenderer::VoxelRenderer() {}

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    static constexpr float Vertices[] = {
        //  x     y     z     u     v
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 1
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 2
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 3

        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 4
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 5
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 6
    };

    static constexpr int32_t Attributes[] = {
        3, 2, 0 // null terminator
    };

    return std::make_shared<graphics::Mesh>(Vertices, 6, Attributes);
}

} // namespace graphics
