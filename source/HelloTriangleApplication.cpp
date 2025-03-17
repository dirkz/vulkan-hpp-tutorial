#include "HelloTriangleApplication.h"

#include "BufferTransfer.h"
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

const std::vector<Vertex> Vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> Indices = {0, 1, 2, 2, 3, 0};

HelloTriangleApplication::HelloTriangleApplication(std::filesystem::path shaderPath)
    : m_shaderPath{shaderPath}, m_currentFrame{0}, m_framebufferResized{false}
{
}

void HelloTriangleApplication::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void HelloTriangleApplication::Resize(int width, int height)
{
    m_framebufferResized = true;
}

void HelloTriangleApplication::InitWindow()
{
    m_window.reset(new Window{"Vulkan", this, Width, Height});
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

    vector<const char *> extensions = RequiredDeviceExtensions();
    vk::DeviceCreateInfo deviceCreateInfo{
        {}, deviceQueueCreateInfos, {}, extensions, &deviceFeatures};

    m_device = m_physicalDevice.createDeviceUnique(deviceCreateInfo);

    m_graphicsQueue = m_device->getQueue(m_familyIndices->GraphicsFamily().value(), 0);
    m_presentQueue = m_device->getQueue(m_familyIndices->PresentFamily().value(), 0);
}

void HelloTriangleApplication::CreateVma()
{
    m_vma.Reset(m_instance.get(), m_physicalDevice, m_device.get());
}

void HelloTriangleApplication::CreateSwapChain()
{
    m_swapchain.reset(
        new SwapChain{m_window.get(), m_physicalDevice, m_device.get(), m_surface.get()});
}

void HelloTriangleApplication::ReCreateSwapChain()
{
    m_window->WaitForDimensionsGreaterZero();
    m_device->waitIdle();

    CreateSwapChain();
    CreateFrameBuffers();
}

void HelloTriangleApplication::CreateRenderPass()
{
    vk::AttachmentDescription colorAttachment{{},
                                              m_swapchain->Format(),
                                              vk::SampleCountFlagBits::e1,
                                              vk::AttachmentLoadOp::eClear,
                                              vk::AttachmentStoreOp::eStore,
                                              vk::AttachmentLoadOp::eDontCare,
                                              vk::AttachmentStoreOp::eDontCare,
                                              vk::ImageLayout::eUndefined,
                                              vk::ImageLayout::ePresentSrcKHR};

    vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};

    vk::SubpassDescription subpass{{}, vk::PipelineBindPoint::eGraphics, {}, {colorAttachmentRef}};

    vk::PipelineStageFlags srcPipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::PipelineStageFlags dstPipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::AccessFlags srcAccessFlags{vk::AccessFlagBits::eNone};
    vk::AccessFlags dstAccessFlags{vk::AccessFlagBits::eColorAttachmentWrite};
    vk::SubpassDependency dependency{vk::SubpassExternal,   0,
                                     srcPipelineStageFlags, dstPipelineStageFlags,
                                     srcAccessFlags,        dstAccessFlags};

    vk::RenderPassCreateInfo createInfo{{}, {colorAttachment}, {subpass}, {dependency}};

    m_renderPass = m_device->createRenderPassUnique(createInfo);
}

void HelloTriangleApplication::CreateGraphicsPipeline()
{
    ShaderModule vertexShader{m_device.get(), m_shaderPath, "shader.vert.spv"};
    ShaderModule fragmentShader{m_device.get(), m_shaderPath, "shader.frag.spv"};

    vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{
        {}, vk::ShaderStageFlagBits::eVertex, vertexShader.Module(), "main"};
    vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{
        {}, vk::ShaderStageFlagBits::eFragment, fragmentShader.Module(), "main"};

    std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStages{
        vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};

    std::vector<vk::DynamicState> dynamicStates{vk::DynamicState::eViewport,
                                                vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamicCreateInfo{{}, dynamicStates};

    auto bindingDescriptions = {Vertex::BindingDescription()};
    auto attributeDescriptions = Vertex::AttributeDescriptions();
    vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo{
        {}, bindingDescriptions, attributeDescriptions};

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{
        {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

    vk::Extent2D swapchainExtent = m_swapchain->Extent();
    vk::Viewport viewport{
        0, 0, static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height),
        0, 1};

    vk::Rect2D scissor{{0, 0}, swapchainExtent};

    vk::PipelineViewportStateCreateInfo viewportCreateInfo{{}, 1, nullptr, 1, nullptr};

    vk::PipelineRasterizationStateCreateInfo rasterizationCreateInfo{{},
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

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

    vk::PipelineColorBlendStateCreateInfo colorBlendCreateInfo{
        {}, false, vk::LogicOp::eCopy, {colorBlendAttachment}, {0.f, 0.f, 0.f, 0.f}};

    vk::PipelineLayoutCreateInfo layoutCreateInfo{};

    m_pipelineLayout = m_device->createPipelineLayoutUnique(layoutCreateInfo);

    vk::GraphicsPipelineCreateInfo pipelineCreateInfo{{},
                                                      pipelineShaderStages,
                                                      &vertexInputCreateInfo,
                                                      &inputAssemblyCreateInfo,
                                                      {},
                                                      &viewportCreateInfo,
                                                      &rasterizationCreateInfo,
                                                      &multisamplingCreateInfo,
                                                      {},
                                                      &colorBlendCreateInfo,
                                                      &dynamicCreateInfo,
                                                      m_pipelineLayout.get(),
                                                      m_renderPass.get(),
                                                      0,
                                                      {},
                                                      -1};

    auto [result, pipeline] = m_device->createGraphicsPipelineUnique({}, pipelineCreateInfo);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error{"could not create the graphics pipeline"};
    }
    else
    {
        m_graphicsPipeline = std::move(pipeline);
    }
}

void HelloTriangleApplication::CreateFrameBuffers()
{
    m_swapchain->CreateFrameBuffers(m_device.get(), m_renderPass.get());
}

void HelloTriangleApplication::CreateCommandPool()
{
    vk::CommandPoolCreateInfo createInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                         m_familyIndices->GraphicsFamily().value()};
    m_commandPool = m_device->createCommandPoolUnique(createInfo);
}

void HelloTriangleApplication::CreateVertexBuffer()
{
    auto verticesSize = sizeof(Vertex) * Vertices.size();
    auto indicesSize = sizeof(uint16_t) * Indices.size();

    vk::BufferUsageFlags stagingBufferUsageFlags{vk::BufferUsageFlagBits::eTransferSrc};
    vk::BufferUsageFlags deviceLocalVertexBufferUsageFlags{vk::BufferUsageFlagBits::eTransferDst |
                                                           vk::BufferUsageFlagBits::eVertexBuffer};
    vk::BufferUsageFlags deviceLocalIndexBufferUsageFlags{vk::BufferUsageFlagBits::eTransferDst |
                                                          vk::BufferUsageFlagBits::eIndexBuffer};

    MappedBuffer stagingVertexBuffer = m_vma.CreateMappedBuffer(
        verticesSize, stagingBufferUsageFlags, vk::SharingMode::eExclusive);
    memcpy(stagingVertexBuffer.Mapped(), Vertices.data(), verticesSize);

    MappedBuffer stagingIndexBuffer =
        m_vma.CreateMappedBuffer(indicesSize, stagingBufferUsageFlags, vk::SharingMode::eExclusive);
    memcpy(stagingIndexBuffer.Mapped(), Indices.data(), indicesSize);

    VmaBuffer deviceLocalVertexBuffer = m_vma.CreateDeviceLocalBuffer(
        verticesSize, deviceLocalVertexBufferUsageFlags, vk::SharingMode::eConcurrent,
        m_familyIndices->UniqueGraphicsAndTransfer());

    VmaBuffer deviceLocalIndexBuffer = m_vma.CreateDeviceLocalBuffer(
        indicesSize, deviceLocalIndexBufferUsageFlags, vk::SharingMode::eConcurrent,
        m_familyIndices->UniqueGraphicsAndTransfer());

    BufferTransfer transfer{*m_device, m_familyIndices.get()};
    transfer.Copy(stagingVertexBuffer, deviceLocalVertexBuffer);
    transfer.Copy(stagingIndexBuffer, deviceLocalIndexBuffer);
    transfer.FinishAndWait();

    m_vertexBuffer.reset(new VmaBuffer{std::move(deviceLocalVertexBuffer)});
    m_indexBuffer.reset(new VmaBuffer{std::move(deviceLocalIndexBuffer)});
}

void HelloTriangleApplication::CreateFrameData()
{
    for (auto i = 0; i < m_frameDatas.size(); ++i)
    {
        m_frameDatas[i] = FrameData{m_device.get(), m_commandPool.get()};
    }
}

void HelloTriangleApplication::RecordCommandBuffer(vk::CommandBuffer commandBuffer,
                                                   uint32_t imageIndex)
{
    vk::CommandBufferBeginInfo beginInfo{};
    commandBuffer.begin(beginInfo);

    vk::Extent2D extent = m_swapchain->Extent();

    vk::ClearValue clearColor{vk::ClearColorValue{0.f, 0.f, 0.f, 1.f}};
    vk::Rect2D area{{0, 0}, extent};
    vk::RenderPassBeginInfo renderPassInfo{
        m_renderPass.get(), m_swapchain->FrameBuffer(imageIndex), area, {clearColor}};

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline.get());
    commandBuffer.bindVertexBuffers(0, {m_vertexBuffer->Buffer()}, {0});
    commandBuffer.bindIndexBuffer(m_indexBuffer->Buffer(), 0, vk::IndexType::eUint16);

    vk::Viewport viewport{0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height),
                          0, 1};
    commandBuffer.setViewport(0, {viewport});

    vk::Rect2D scissor{{0, 0}, extent};
    commandBuffer.setScissor(0, {scissor});

    commandBuffer.drawIndexed(Indices.size(), 1, 0, 0, 0);

    commandBuffer.endRenderPass();

    commandBuffer.end();
}

void HelloTriangleApplication::InitVulkan()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateVma();
    CreateSwapChain();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFrameBuffers();
    CreateCommandPool();
    CreateVertexBuffer();
    CreateFrameData();
}

void HelloTriangleApplication::MainLoop()
{
    m_window->Loop([this]() { this->DrawFrame(); });
    m_device->waitIdle();
}

void HelloTriangleApplication::DrawFrame()
{
    FrameData &frameData = m_frameDatas[m_currentFrame].value();
    vk::Fence fence = frameData.InFlightFence();
    vk::Result result = m_device->waitForFences({fence}, VK_TRUE, UINT64_MAX);
    assert(result == vk::Result::eSuccess);

    vk::SwapchainKHR swapchain = **m_swapchain;

    uint32_t imageIndex;
    result = m_device->acquireNextImageKHR(
        swapchain, UINT64_MAX, frameData.ImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        ReCreateSwapChain();
        return;
    }

    assert(result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR);

    m_device->resetFences({fence});

    frameData.CommandBuffer().reset();
    RecordCommandBuffer(frameData.CommandBuffer(), imageIndex);

    vk::PipelineStageFlags pipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo submitInfo{{frameData.ImageAvailableSemaphore()},
                              pipelineStageFlags,
                              {frameData.CommandBuffer()},
                              {frameData.RenderFinishedSemaphore()}};
    m_graphicsQueue.submit(submitInfo, frameData.InFlightFence());

    vk::PresentInfoKHR presentInfo{
        {frameData.RenderFinishedSemaphore()}, {swapchain}, {imageIndex}};

    result = m_presentQueue.presentKHR(presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||
        m_framebufferResized)
    {
        m_framebufferResized = false;
        ReCreateSwapChain();
    }

    assert(result == vk::Result::eSuccess || result == vk::Result::eErrorOutOfDateKHR ||
           result == vk::Result::eSuboptimalKHR);

    m_currentFrame = (m_currentFrame + 1) % MaxFramesInFlight;
}

void HelloTriangleApplication::Cleanup()
{
}

} // namespace zvk