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
    vector<const char *> extensions = RequiredInstanceExtensions();

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

void HelloTriangleApplication::CreateSurface()
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(m_instance.get(), m_window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }

    m_surface = vk::UniqueSurfaceKHR{surface, m_instance.get()};
}

bool HelloTriangleApplication::IsDeviceSuitable(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices{device, m_surface.get()};
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
    m_familyIndices.reset(new QueueFamilyIndices{m_physicalDevice, m_surface.get()});
}

void HelloTriangleApplication::CreateLogicalDevice()
{
    vector<uint32_t> queueFamilies = m_familyIndices->UniqueFamilies();

    float priority = 1.0f;
    vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos(queueFamilies.size());
    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        uint32_t family = queueFamilies[i];
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo{{}, family, 1, &priority};
        deviceQueueCreateInfos[i] = deviceQueueCreateInfo;
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vk::DeviceCreateInfo deviceCreateInfo{{}, deviceQueueCreateInfos, {}, {}, &deviceFeatures};

    m_device = m_physicalDevice.createDeviceUnique(deviceCreateInfo);

    m_graphicsQueue = m_device->getQueue(m_familyIndices->GraphicsFamily().value(), 0);
    m_presentQueue = m_device->getQueue(m_familyIndices->PresentFamily().value(), 0);
}

void HelloTriangleApplication::InitVulkan()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
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