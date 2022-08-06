#include <window/Events.hpp>

namespace window
{

static constexpr uint32_t MouseButtons = 12;

static constexpr uint32_t NumberOfKeys = 1024;

std::pair<bool, uint32_t> Events::keys[NumberOfKeys]{{false, 0}};

std::pair<bool, uint32_t> Events::mouseButtons[MouseButtons]{{false, 0}};

uint32_t Events::current = 0;

float Events::deltaX = 0;

float Events::deltaY = 0;

float Events::x = 0;

float Events::y = 0;

void Events::cursorCallback(GLFWwindow *window, double xPos, double yPos)
{
    Events::deltaX += xPos - Events::x;
    Events::deltaY += yPos - Events::y;
}

void Events::windowCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Events::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        Events::mouseButtons[button].first = true;
        Events::mouseButtons[button].second = Events::current;
        break;
    case GLFW_RELEASE:
        Events::mouseButtons[button].first = false;
        Events::mouseButtons[button].second = Events::current;
        break;
    }
}

void Events::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        Events::keys[key].first = true;
        Events::keys[key].second = Events::current;
        break;
    case GLFW_RELEASE:
        Events::keys[key].first = false;
        Events::keys[key].second = Events::current;
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
    ++Events::current;
    Events::deltaX = Events::deltaY = 0;

    glfwPollEvents();
}

bool Events::Pressed(int32_t key)
{
    if (!key || key >= NumberOfKeys)
    {
        return false;
    }

    return Events::keys[key].first && Events::current == Events::keys[key].second;
}

bool Events::JPressed(int32_t key)
{
    if (!key || key >= NumberOfKeys)
    {
        return false;
    }

    return Events::keys[key].first;
}

} // namespace window
