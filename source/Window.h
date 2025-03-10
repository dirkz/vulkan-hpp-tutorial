#pragma once

#include "stdafx.h"

namespace zvk
{

struct Window
{
    Window(const std::string &title, uint32_t width, uint32_t height);
    ~Window();

    std::vector<const char *> RequiredExtensions() const;
    vk::UniqueSurfaceKHR CreateSurface(vk::Instance instance);
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const;

    template <class T> void Loop(const T &callback)
    {
        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();
            callback();
        }
    }

  private:
    GLFWwindow *m_window;
};

} // namespace zvk
