#include <window/Camera.hpp>
#include <window/Events.hpp>
#include <window/Window.hpp>

#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>

#include <iostream>

static constexpr float Vertices[] = {
    //  x     y     z     u     v
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 1
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 2
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 3

    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 4
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 5
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6
};

int main(int argc, char const *argv[])
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;

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

    glClearColor(0.17, 0.17, 0.17, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    std::shared_ptr<window::Camera> camera =
        std::make_shared<window::Camera>(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(40.0f));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

    try
    {
        while (!window::Window::ShouldClose())
        {
            if (window::Events::JPressed(GLFW_KEY_ESCAPE))
            {
                window::Window::ShouldClose(true);
            }

            if (window::Events::Pressed(GLFW_KEY_S))
            {
                glm::vec3 position = camera->GetPosition();
                position.z -= 0.002f;
                camera->SetPosition(position);
            }
            if (window::Events::Pressed(GLFW_KEY_W))
            {
                glm::vec3 position = camera->GetPosition();
                position.z += 0.002f;
                camera->SetPosition(position);
            }
            if (window::Events::Pressed(GLFW_KEY_A))
            {
                glm::vec3 position = camera->GetPosition();
                position.x -= 0.006f;
                camera->SetPosition(position);
            }
            if (window::Events::Pressed(GLFW_KEY_D))
            {
                glm::vec3 position = camera->GetPosition();
                position.x += 0.006f;
                camera->SetPosition(position);
            }

            glClear(GL_COLOR_BUFFER_BIT);

            shader->Use();
            shader->UniformMatrix("model", model);
            shader->UniformMatrix("projView", camera->GetProjection() * camera->GetView());
            texture->Bind();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);

            window::Events::PollEvents();
            window::Window::SwapBuffer();
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
