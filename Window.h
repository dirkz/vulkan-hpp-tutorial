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
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);
    void Loop();

  private:
    GLFWwindow *m_window;
};

} // namespace zvk
