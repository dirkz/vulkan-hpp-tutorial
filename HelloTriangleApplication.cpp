#include "HelloTriangleApplication.h"

#include "Constants.h"
#include "Extensions.h"
#include "Validation.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

using namespace std;

namespace zvk
{

static const uint32_t Width = 800;
static const uint32_t Height = 600;

HelloTriangleApplication::HelloTriangleApplication() : m_window{nullptr}
{
}

void HelloTriangleApplication::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void HelloTriangleApplication::InitWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(Width, Height, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::CreateInstance()
{
    vector<const char *> extensions = RequiredExtensions();

    vk::ApplicationInfo appInfo{"Hello Triangle", VK_MAKE_API_VERSION(0, 0, 1, 0), "No Engine",
                                VK_MAKE_API_VERSION(0, 0, 1, 0), VK_API_VERSION_1_0};

    vector<const char *> validationLayers = EnabledValidationLayers();

    vk::InstanceCreateInfo createInfo{{}, &appInfo, validationLayers, extensions};

    if (EnableValidationLayers)
    {
        m_standaloneDebugMessenger.reset(new DebugUtilsMessenger{});
        vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> c{
            createInfo, m_standaloneDebugMessenger->CreateInfo()};

        // Manually follow the pNext chain and assert it looks correct.
        vk::InstanceCreateInfo createInfo = c.get<vk::InstanceCreateInfo>();
        const vk::DebugUtilsMessengerCreateInfoEXT *debugCreateInfo =
            static_cast<const vk::DebugUtilsMessengerCreateInfoEXT *>(createInfo.pNext);
        assert(debugCreateInfo);

        m_instance = vk::createInstanceUnique(c.get<vk::InstanceCreateInfo>());
    }
    else
    {
        m_instance = vk::createInstanceUnique(createInfo);
    }

    VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
}

void HelloTriangleApplication::SetupDebugMessenger()
{
    if (EnableValidationLayers)
    {
        // Create the debug messenger for the instance.
        m_instanceDebugMessenger.reset(new DebugUtilsMessenger{m_instance.get()});
    }
}

bool HelloTriangleApplication::IsDeviceSuitable(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices{device};
    return indices.IsComplete();
}

void HelloTriangleApplication::PickPhysicalDevice()
{
    vk::PhysicalDevice result;

    vector<vk::PhysicalDevice> devices = m_instance->enumeratePhysicalDevices();
    for (auto device : devices)
    {
        if (IsDeviceSuitable(device))
        {
            result = device;
            break;
        }
    }

    if (!result)
    {
        throw runtime_error{"no suitable device found"};
    }

    m_physicalDevice = result;
    m_familyIndices.reset(new QueueFamilyIndices{m_physicalDevice});
}

void HelloTriangleApplication::CreateLogicalDevice()
{
}

void HelloTriangleApplication::InitVulkan()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    CreateInstance();
    SetupDebugMessenger();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void HelloTriangleApplication::MainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::Cleanup()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace zvk