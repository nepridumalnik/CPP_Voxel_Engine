#include <window/Window.hpp>

#include <stdexcept>

namespace window
{

GLFWwindow *Window::window_ = nullptr;
int32_t Window::width_ = 0;
int32_t Window::height_ = 0;
bool Window::cursorLocker_ = false;

void Window::Init(uint32_t w, uint32_t h, const std::string &windowName)
{
    if (GLFW_FALSE == glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Window::width_ = w;
    Window::height_ = h;

    window_ =
        glfwCreateWindow(Window::width_, Window::height_, windowName.c_str(), nullptr, nullptr);

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

    glViewport(0, 0, Window::width_, Window::height_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

int32_t Window::GetWidth()
{
    return Window::width_;
}

int32_t Window::GetHeight()
{
    return Window::height_;
}

void Window::SetWidth(int32_t width)
{
    Window::width_ = width;
}

void Window::SetHeight(int32_t height)
{
    Window::height_ = height;
}

void Window::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Window::setCursorMode(int32_t mode)
{
    glfwSetInputMode(window_, GLFW_CURSOR, mode);
}

} // namespace window
