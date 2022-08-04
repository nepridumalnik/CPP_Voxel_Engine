#pragma once

#include <window/Window.hpp>

#include <utils/macros.hpp>

namespace window
{

class EXPORT Events
{
public:
    static std::pair<bool, uint32_t> keys[];
    static std::pair<bool, uint32_t> mouseButtons[];
    static uint32_t current;
    static float deltaX;
    static float deltaY;
    static float x;
    static float y;

public:
    static void Init();
    static void PollEvents();
    static bool Pressed(int32_t key);
    static bool JPressed(int32_t key);

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow *window, double xPos, double yPos);
};

} // namespace window
