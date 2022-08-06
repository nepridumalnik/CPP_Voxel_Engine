#include <window/Camera.hpp>
#include <window/Events.hpp>
#include <window/Window.hpp>

#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>

#include <iostream>

static constexpr float Vertices[] = {
    //  x     y     z     u     v
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 1
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 2
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 3

    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 4
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 5
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 6
};

int main(int argc, char const *argv[])
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;
    static constexpr float speed = 0.5f;
    static constexpr float rotationLimit = 89.99f;
    static constexpr float mouseSpeed = 0.5f;

    window::Window::Init(width, height, "Title");
    window::Events::Init();

    std::shared_ptr<graphics::Shader> shader = graphics::LoadShader("main.vert", "main.frag");
    std::shared_ptr<graphics::Texture> texture = graphics::LoadTexture("checker.jpg");

    uint32_t VAO;
    uint32_t VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glClearColor(0.2, 0.2, 0.2, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    std::shared_ptr<window::Camera> camera =
        std::make_shared<window::Camera>(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(75.0f));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

    float lastTime = glfwGetTime();
    float delta = 0.0f;
    float camX = 0.0f;
    float camY = 0.0f;

    bool cursorLocked = true;
    window::Events::SetCursorLocked(cursorLocked);

    try
    {
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

            glClear(GL_COLOR_BUFFER_BIT);

            shader->Use();
            shader->UniformMatrix("model", model);
            shader->UniformMatrix("projection", camera->GetProjection());
            shader->UniformMatrix("view", camera->GetView());
            texture->Bind();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            window::Window::SwapBuffer();
            window::Events::PollEvents();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    window::Window::Terminate();

    return 0;
}
