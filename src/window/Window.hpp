#pragma once

#include <utils/macros.hpp>

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

private:
    static GLFWwindow *window_;

    friend class Events;
};

} // namespace window
