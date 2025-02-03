#include "stdafx.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

using namespace std;
using namespace vk;

namespace vkz
{

static const uint32_t Width = 800;
static const uint32_t Height = 600;

struct HelloTriangleApplication
{
    void Run()
    {
        VULKAN_HPP_DEFAULT_DISPATCHER.init();

        InitWindow();
        InitVulkan();
        MainLoop();
        Cleanup();
    }

  private:
    GLFWwindow *m_window;

    void InitWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(Width, Height, "Vulkan", nullptr, nullptr);
    }

    vector<const char *> WindowExtensions()
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

    void CreateInstance()
    {
        vector<const char *> extensions = WindowExtensions();

        ApplicationInfo appInfo{"Hello Triangle", VK_MAKE_API_VERSION(0, 0, 1, 0), "No Engine",
                                VK_MAKE_API_VERSION(0, 0, 1, 0), VK_API_VERSION_1_0};

        InstanceCreateInfo createInfo{{}, &appInfo, {}, extensions};
        vk::UniqueInstance instance = createInstanceUnique(createInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());
    }

    void InitVulkan()
    {
        CreateInstance();
    }

    void MainLoop()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();
        }
    }

    void Cleanup()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
};

} // namespace vkz

int main()
{
    vkz::HelloTriangleApplication app{};

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}