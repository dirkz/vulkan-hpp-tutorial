#include "HelloTriangleApplication.h"

#include "Constants.h"
#include "Extensions.h"
#include "ShaderModule.h"
#include "SwapChainSupportDetails.h"
#include "Validation.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

using namespace std;

namespace zvk
{

static const uint32_t Width = 800;
static const uint32_t Height = 600;

HelloTriangleApplication::HelloTriangleApplication(std::filesystem::path shaderPath)
    : m_shaderPath{shaderPath}
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
    m_window.reset(new Window{"Vulkan", Width, Height});
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
    m_surface = m_window->CreateSurface(m_instance.get());
}

bool HelloTriangleApplication::IsDeviceSuitable(vk::PhysicalDevice device) const
{
    QueueFamilyIndices indices{device, m_surface.get()};
    if (!indices.IsComplete())
    {
        return false;
    }

    if (!CheckDeviceExtensionSupport(device))
    {
        return false;
    }

    SwapChainSupportDetails swapChainDetails{device, m_surface.get()};
    return swapChainDetails.IsAdequate();
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
}

void HelloTriangleApplication::CreateLogicalDevice()
{
    QueueFamilyIndices familyIndices{m_physicalDevice, m_surface.get()};
    vector<uint32_t> queueFamilies = familyIndices.UniqueFamilies();

    float priority = 1.0f;
    vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos(queueFamilies.size());
    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        uint32_t family = queueFamilies[i];
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo{{}, family, 1, &priority};
        deviceQueueCreateInfos[i] = deviceQueueCreateInfo;
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vector<const char *> extensions = RequiredDeviceExtensions();
    vk::DeviceCreateInfo deviceCreateInfo{
        {}, deviceQueueCreateInfos, {}, extensions, &deviceFeatures};

    m_device = m_physicalDevice.createDeviceUnique(deviceCreateInfo);

    m_graphicsQueue = m_device->getQueue(familyIndices.GraphicsFamily().value(), 0);
    m_presentQueue = m_device->getQueue(familyIndices.PresentFamily().value(), 0);
}

void HelloTriangleApplication::CreateSwapChain()
{
    m_swapchain.reset(
        new SwapChain{m_window.get(), m_physicalDevice, m_device.get(), m_surface.get()});
}

void HelloTriangleApplication::CreateGraphicsPipeline()
{
    ShaderModule vertexShader{m_device.get(), m_shaderPath, "shader.vert.spv"};
    ShaderModule fragmentShader{m_device.get(), m_shaderPath, "shader.frag.spv"};

    vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{
        {}, vk::ShaderStageFlagBits::eVertex, vertexShader.Module(), "main"};
    vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{
        {}, vk::ShaderStageFlagBits::eFragment, fragmentShader.Module(), "main"};

    std::vector<vk::DynamicState> dynamicStates{vk::DynamicState::eViewport,
                                                vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{{}, dynamicStates};

    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{
        {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

    vk::Extent2D swapchainExtent = m_swapchain->Extent();
    vk::Viewport viewport{
        0, 0, static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height),
        0, 1};

    vk::Rect2D scissor{{0, 0}, swapchainExtent};

    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo{{}, 1, nullptr, 1, nullptr};

    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{
        {},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        VK_FALSE,
        0,
        0,
        0,
        1};

    vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo{};
}

void HelloTriangleApplication::InitVulkan()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateGraphicsPipeline();
}

void HelloTriangleApplication::MainLoop()
{
    m_window->Loop();
}

void HelloTriangleApplication::Cleanup()
{
}

} // namespace zvk