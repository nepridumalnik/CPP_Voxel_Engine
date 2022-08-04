#include <window/Events.hpp>
#include <window/Window.hpp>

#include <graphics/Shader.hpp>

#include <iostream>

static constexpr float Vertices[] = {
    0, 0, 0, // 1
    1, 0, 0, // 2
    0, 1, 0, // 3
};

int main(int argc, char const *argv[])
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;

    window::Window::Init(width, height, "Title");
    window::Events::Init();

    std::shared_ptr<graphics::Shader> shader = graphics::LoadShader("main.vert", "main.frag");

    uint32_t VAO;
    uint32_t VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glClearColor(0.17, 0.17, 0.17, 1);

    try
    {
        while (!window::Window::ShouldClose())
        {
            window::Events::PollEvents();

            if (window::Events::JPressed(GLFW_KEY_ESCAPE))
            {
                window::Window::ShouldClose(true);
            }

            glClear(GL_COLOR_BUFFER_BIT);

            shader->Use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);

            window::Window::SwapBuffer();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    window::Window::Terminate();

    return 0;
}
