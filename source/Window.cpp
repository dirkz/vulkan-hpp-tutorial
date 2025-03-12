#include "Window.h"

using namespace std;

namespace zvk
{

static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Window::Callback *callback =
        reinterpret_cast<Window::Callback *>(glfwGetWindowUserPointer(window));
    callback->Resize(width, height);
}

Window::Window(const std::string &title, Callback *callback, uint32_t width, uint32_t height)
    : m_callback{callback}
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, callback);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}

vector<const char *> Window::RequiredExtensions() const
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

vk::Extent2D Window::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const
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

} // namespace zvk