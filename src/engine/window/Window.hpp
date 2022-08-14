#pragma once

#include <string>

struct GLFWwindow;

namespace window
{

class EXPORT Window
{
public:
    static void Init(uint32_t w, uint32_t h, const std::string &windowName);
    static void Terminate();
    static void SwapBuffer();
    static bool ShouldClose();
    static void ShouldClose(bool flag);
    static int32_t GetWidth();
    static int32_t GetHeight();
    static void SetWidth(int32_t width);
    static void SetHeight(int32_t height);
    static void SetClearColor(float r, float g, float b, float a);

private:
    static void setCursorMode(int32_t mode);

private:
    static GLFWwindow *window_;
    static int32_t width_;
    static int32_t height_;
    static bool cursorLocker_;

    friend class Events;
    friend class Camera;
};

} // namespace window
