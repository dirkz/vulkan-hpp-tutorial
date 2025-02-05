#include "Window.h"

using namespace std;

namespace zvk
{

Window::Window(const std::string &title, uint32_t width, uint32_t height)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}

vector<const char *> Window::RequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vector<const char *> extensions(glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; ++i)
    {
        extensions[i] = glfwExtensions[i];
    }

    return extensions;
}

vk::UniqueSurfaceKHR Window::CreateSurface(vk::Instance instance)
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }

    return vk::UniqueSurfaceKHR{surface, instance};
}

vk::Extent2D Window::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    uint32_t w = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width,
                            capabilities.maxImageExtent.width);
    uint32_t h = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height,
                            capabilities.maxImageExtent.height);

    return vk::Extent2D{w, h};
}

void Window::Loop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

} // namespace zvk