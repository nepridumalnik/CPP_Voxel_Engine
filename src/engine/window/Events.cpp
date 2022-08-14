#include <window/Events.hpp>

namespace window
{

static constexpr uint32_t MouseButtons = 12;

static constexpr uint32_t NumberOfKeys = 1024;

std::pair<bool, uint32_t> Events::keys_[NumberOfKeys]{{false, 0}};
std::pair<bool, uint32_t> Events::mouseButtons_[MouseButtons]{{false, 0}};
uint32_t Events::current_ = 0;
float Events::deltaX_ = 0;
float Events::deltaY = 0;
float Events::x_ = 0;
float Events::y_ = 0;
bool Events::cursorLocked_ = false;

void Events::cursorCallback(GLFWwindow *window, double xPos, double yPos)
{
    Events::deltaX_ += xPos - Events::x_;
    Events::deltaY += yPos - Events::y_;
    Events::x_ = xPos;
    Events::y_ = yPos;
}

void Events::windowCallback(GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);

    Window::width_ = width;
    Window::height_ = height;
}

void Events::mouseCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        Events::mouseButtons_[button].first = true;
        Events::mouseButtons_[button].second = Events::current_;
        break;
    case GLFW_RELEASE:
        Events::mouseButtons_[button].first = false;
        Events::mouseButtons_[button].second = Events::current_;
        break;
    }
}

void Events::keyCallback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action,
                         int32_t mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        Events::keys_[key].first = true;
        Events::keys_[key].second = Events::current_;
        break;
    case GLFW_RELEASE:
        Events::keys_[key].first = false;
        Events::keys_[key].second = Events::current_;
        break;
    }
}

void Events::Init()
{
    glfwSetKeyCallback(Window::window_, Events::keyCallback);
    glfwSetMouseButtonCallback(Window::window_, Events::mouseCallback);
    glfwSetCursorPosCallback(Window::window_, Events::cursorCallback);
    glfwSetWindowSizeCallback(Window::window_, Events::windowCallback);
}

void Events::PollEvents()
{
    ++Events::current_;
    Events::deltaX_ = Events::deltaY = 0;

    glfwPollEvents();
}

bool Events::Pressed(int32_t key)
{
    if (NumberOfKeys < key && key >= 0)
    {
        return false;
    }

    return Events::keys_[key].first;
}

bool Events::JPressed(int32_t key)
{
    if (NumberOfKeys < key && key >= 0)
    {
        return false;
    }

    return Events::keys_[key].first && Events::current_ == Events::keys_[key].second;
}

bool Events::Clicked(int32_t key)
{
    if (MouseButtons < key && key >= 0)
    {
        return false;
    }

    return Events::mouseButtons_[key].first;
}

bool Events::JClicked(int32_t key)
{
    if (MouseButtons < key && key >= 0)
    {
        return false;
    }

    return Events::mouseButtons_[key].first &&
           Events::current_ == Events::mouseButtons_[key].second;
}

float Events::GetDeltaX()
{
    return Events::deltaX_;
}

float Events::GetDeltaY()
{
    return Events::deltaY;
}

void Events::SetCursorLocked(bool flag)
{
    Events::cursorLocked_ = flag;
    Window::setCursorMode(Events::cursorLocked_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Events::IsCursorLocked()
{
    return Events::cursorLocked_;
}

} // namespace window
