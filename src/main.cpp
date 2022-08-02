#include <window/Events.hpp>
#include <window/Window.hpp>

#include <graphics/Shader.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    constexpr uint32_t width = 1280;
    constexpr uint32_t height = 720;

    Window::Init(width, height, "Title");
    Events::Init();

    std::shared_ptr<Shader> shader = Shader::LoadShader("main.vert", "main.frag");

    try
    {
        while (!Window::ShouldClose())
        {
            Events::PollEvents();

            if (Events::JPressed(GLFW_KEY_ESCAPE))
            {
                Window::ShouldClose(true);
            }

            Window::SwapBuffer();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    Window::Terminate();

    return 0;
}
