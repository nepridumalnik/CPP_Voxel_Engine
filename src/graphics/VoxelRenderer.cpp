#include <graphics/VoxelRenderer.hpp>

int chunk_attrs[] = {3, 2, 1, 0};

static constexpr uint32_t VertexSize = 3 + 2 + 1;
static constexpr int32_t ChunkAttributes[] = {3, 2, 1, 0};

namespace graphics
{

const uint32_t VoxelRenderer::vertexSize_ = 3 * 2;
const uint32_t VoxelRenderer::numberOfSides_ = 6;

VoxelRenderer::VoxelRenderer(uint32_t capacity)
{
    buffer_.resize(capacity * vertexSize_ * numberOfSides_);
    buffer_.shrink_to_fit();
}

std::shared_ptr<Mesh> VoxelRenderer::Render(std::shared_ptr<voxels::Chunk> chunk)
{
    return nullptr;
}

} // namespace graphics
