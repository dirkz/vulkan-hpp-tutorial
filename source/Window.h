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

    template <class T> void Loop(const T &drawCallback)
    {
        while (!glfwWindowShouldClose(m_window))
        {
            int escapeStatus = glfwGetKey(m_window, GLFW_KEY_ESCAPE);
            if (escapeStatus == GLFW_PRESS)
            {
                break;
            }
            glfwPollEvents();
            drawCallback();
        }
    }

  private:
    GLFWwindow *m_window;
};

} // namespace zvk
