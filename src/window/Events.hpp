#pragma once

#include <window/Window.hpp>

namespace window
{

class EXPORT Events
{
public:
    static void Init();
    static void PollEvents();
    static bool Pressed(int32_t key);
    static bool JPressed(int32_t key);
    static float GetDeltaX();
    static float GetDeltaY();
    static void SetCursorLocked(bool flag);
    static bool IsCursorLocked();

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow *window, double xPos, double yPos);
    static void windowCallback(GLFWwindow *window, int width, int height);

private:
    static std::pair<bool, uint32_t> keys_[];
    static std::pair<bool, uint32_t> mouseButtons_[];
    static uint32_t current_;
    static float deltaX_;
    static float deltaY;
    static float x_;
    static float y_;
    static bool cursorLocked_;
};

} // namespace window
