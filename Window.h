#pragma once

#include "stdafx.h"

namespace zvk
{

struct Window
{
    Window(const std::string &title, uint32_t width, uint32_t height);
    ~Window();

    std::vector<const char *> RequiredExtensions();
    vk::UniqueSurfaceKHR CreateSurface(vk::Instance instance);

  private:
    GLFWwindow *m_window;
};

} // namespace zvk
