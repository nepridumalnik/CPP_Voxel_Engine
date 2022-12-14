#include <graphics/Mesh.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>

#include <window/Camera.hpp>
#include <window/Events.hpp>
#include <window/Window.hpp>

#include <voxels/Chunk.hpp>
#include <voxels/Chunks.hpp>
#include <voxels/VoxelRenderer.hpp>
#include <voxels/voxel.hpp>

#include <utils/loaders.hpp>

#include <iostream>
#include <vector>

static constexpr float Vertices[] = {
    -0.01f, -0.01f, // 1
    0.01f,  0.01f,  // 2

    -0.01f, 0.01f,  // 3
    0.01f,  -0.01f, // 4
};

int32_t Attributes[] = {
    2, 0 // null terminator
};

int32_t main(int32_t argc, char const *argv[])
try
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;
    static constexpr float speed = 5.0f;
    static constexpr float rotationLimit = 89.99f;
    static constexpr float mouseSpeed = 0.8f;
    static constexpr float epsilon = 0.5f;
    static constexpr float touchLength = 5.0f;

    window::Window::Init(width, height, "Window");
    window::Window::SetClearColor(0.27f, 0.27f, 0.27f, 1);

    window::Events::Init();

    std::shared_ptr<graphics::Shader> crosshairShader =
        graphics::LoadShader("crosshair.vert", "crosshair.frag");
    std::shared_ptr<graphics::Mesh> crosshairMesh =
        std::make_shared<graphics::Mesh>(Vertices, sizeof(Vertices) / sizeof(float), Attributes);

    std::shared_ptr<graphics::Shader> shader = graphics::LoadShader("main.vert", "main.frag");
    std::shared_ptr<graphics::Texture> texture = graphics::LoadTexture("block.png");

    std::shared_ptr<voxels::VoxelRenderer> renderer = std::make_shared<voxels::VoxelRenderer>();
    std::shared_ptr<voxels::Chunks> chunks = std::make_shared<voxels::Chunks>(1, 1, 1);
    std::vector<std::shared_ptr<graphics::Mesh>> meshes;
    meshes.resize(chunks->Size());

    std::shared_ptr<window::Camera> camera =
        std::make_shared<window::Camera>(glm::vec3(0.0f, 0.0f, 20.0f), glm::radians(75.0f));

    glm::mat4 model(1.0f);

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    bool cursorLocked = false;
    window::Events::SetCursorLocked(cursorLocked);

    while (!window::Window::ShouldClose())
    {
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        if (window::Events::JPressed(GLFW_KEY_ESCAPE))
        {
            window::Window::ShouldClose(true);
        }
        if (window::Events::JPressed(GLFW_KEY_TAB))
        {
            window::Events::SetCursorLocked(cursorLocked = !cursorLocked);
        }

        if (window::Events::Pressed(GLFW_KEY_W))
        {
            camera->SetPosition(camera->GetPosition() + camera->GetFront() * speed * delta);
        }
        if (window::Events::Pressed(GLFW_KEY_S))
        {
            camera->SetPosition(camera->GetPosition() - camera->GetFront() * speed * delta);
        }
        if (window::Events::Pressed(GLFW_KEY_A))
        {
            camera->SetPosition(camera->GetPosition() - camera->GetRight() * speed * delta);
        }
        if (window::Events::Pressed(GLFW_KEY_D))
        {
            camera->SetPosition(camera->GetPosition() + camera->GetRight() * speed * delta);
        }
        if (window::Events::Pressed(GLFW_KEY_E))
        {
            camera->SetPosition(camera->GetPosition() + camera->GetUp() * speed * delta);
        }
        if (window::Events::Pressed(GLFW_KEY_Q))
        {
            camera->SetPosition(camera->GetPosition() - camera->GetUp() * speed * delta);
        }

        if (window::Events::IsCursorLocked())
        {
            camY -= window::Events::GetDeltaY() / window::Window::GetHeight() * mouseSpeed;
            camX -= window::Events::GetDeltaX() / window::Window::GetHeight() * mouseSpeed;

            if (camY < -glm::radians(rotationLimit))
            {
                camY = -glm::radians(rotationLimit);
            }
            if (camY > glm::radians(rotationLimit))
            {
                camY = glm::radians(rotationLimit);
            }

            camera->SetRotation(glm::mat4(1.0f));
            camera->Rotate(camX, camY, 0);
        }

        glm::vec3 end;
        glm::vec3 norm;
        glm::vec3 iend;

        auto vox = chunks->RayCast(camera->GetPosition(), camera->GetFront(), touchLength, end,
                                   norm, iend);

        if (std::nullopt != vox)
        {
            if (window::Events::JClicked(GLFW_MOUSE_BUTTON_1))
            {
                chunks->SetVoxelByCoord(iend.x, iend.y, iend.z, voxels::BlockType::None);
            }
            else if (window::Events::JClicked(GLFW_MOUSE_BUTTON_2))
            {
                chunks->SetVoxelByCoord(iend.x + norm.x, iend.y + norm.y, iend.z + norm.z,
                                        voxels::BlockType::Dirt);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        shader->UniformMatrix("projview", camera->GetProjection() * camera->GetView());
        texture->Bind();

        for (uint32_t i = 0; i < chunks->Size(); ++i)
        {
            std::shared_ptr<voxels::Chunk> chunk = chunks->At(i);
            if (chunk->RequireUpdate())
            {
                meshes[i] = renderer->Render(chunk, chunks);
            }
        }

        for (uint32_t i = 0; i < chunks->Size(); ++i)
        {
            glm::mat4 model(1.0f);
            std::shared_ptr<voxels::Chunk> chunk = chunks->At(i);
            std::shared_ptr<graphics::Mesh> mesh = meshes.at(i);

            model = glm::translate(model, glm::vec3(chunk->GetX() * voxels::ChunkWidth + epsilon,
                                                    chunk->GetZ() * voxels::ChunkDepth + epsilon,
                                                    chunk->GetY() * voxels::ChunkHeight + epsilon));
            shader->UniformMatrix("model", model);
            mesh->Draw(GL_TRIANGLES);
        }

        crosshairShader->Use();
        crosshairMesh->Draw(GL_LINES);

        window::Window::SwapBuffer();
        window::Events::PollEvents();
    }

    window::Window::Terminate();

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << e.what() << std::endl;
    return -1;
}