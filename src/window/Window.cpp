#include <window/Window.hpp>

#include <stdexcept>

namespace window
{

GLFWwindow *Window::window_ = nullptr;

void Window::Init(uint32_t w, uint32_t h, const std::string &windowName)
{
    if (GLFW_FALSE == glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(w, h, windowName.c_str(), nullptr, nullptr);

    if (nullptr == window_)
    {
        Window::Terminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glViewport(0, 0, w, h);
}

void Window::Terminate()
{
    glfwTerminate();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window_);
}

void Window::ShouldClose(bool flag)
{
    glfwSetWindowShouldClose(window_, flag);
}

void Window::SwapBuffer()
{
    glfwSwapBuffers(window_);
}

} // namespace window
