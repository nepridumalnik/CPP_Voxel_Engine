#include <graphics/Mesh.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/VoxelRenderer.hpp>

#include <window/Camera.hpp>
#include <window/Events.hpp>
#include <window/Window.hpp>

#include <voxels/Chunk.hpp>
#include <voxels/Chunks.hpp>
#include <voxels/voxel.hpp>

#include <utils/loaders.hpp>

#include <iostream>
#include <vector>

int32_t main(int32_t argc, char const *argv[])
try
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;
    static constexpr float speed = 5.0f;
    static constexpr float rotationLimit = 89.99f;
    static constexpr float mouseSpeed = 0.8f;

    window::Window::Init(width, height, "Window");
    window::Window::SetClearColor(0.27f, 0.27f, 0.27f, 1);

    window::Events::Init();

    std::shared_ptr<graphics::Shader> shader = graphics::LoadShader("main.vert", "main.frag");
    std::shared_ptr<graphics::Texture> texture = graphics::LoadTexture("block.png");
    std::shared_ptr<graphics::VoxelRenderer> renderer = std::make_shared<graphics::VoxelRenderer>();

    std::shared_ptr<voxels::Chunks> chunks = std::make_shared<voxels::Chunks>(4, 4, 1);
    std::vector<std::shared_ptr<graphics::Mesh>> meshes;

    meshes.resize(chunks->Size());
    for (uint32_t i = 0; i < chunks->Size(); ++i)
    {
        meshes[i] = renderer->Render(chunks->At(i), chunks);
    }

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        shader->UniformMatrix("projview", camera->GetProjection() * camera->GetView());
        texture->Bind();

        for (uint32_t i = 0; i < chunks->Size(); ++i)
        {
            glm::mat4 model(1.0f);
            std::shared_ptr<voxels::Chunk> chunk = chunks->At(i);
            std::shared_ptr<graphics::Mesh> mesh = meshes.at(i);

            model = glm::translate(model, glm::vec3(chunk->GetX() * voxels::ChunkWidth,
                                                    chunk->GetZ() * voxels::ChunkDepth,
                                                    chunk->GetY() * voxels::ChunkHeight));
            shader->UniformMatrix("model", model);
            mesh->Draw(GL_TRIANGLES);
        }

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