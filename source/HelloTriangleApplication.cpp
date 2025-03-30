#include "HelloTriangleApplication.h"

#include "BufferTransfer.h"
#include "Constants.h"
#include "Extensions.h"
#include "ShaderModule.h"
#include "Strings.h"
#include "SwapChainSupportDetails.h"
#include "Validation.h"
#include "VmaImage.h"

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

HelloTriangleApplication::HelloTriangleApplication(std::filesystem::path shaderPath,
                                                   std::filesystem::path texturePath)
    : m_shaderPath{shaderPath}, m_texturePath{texturePath}, m_currentFrame{0},
      m_framebufferResized{false}
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

void HelloTriangleApplication::CreateDescriptorSetLayout()
{
    vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlagBits::eVertex;

    // Note: Using the "unenhanced" constructor here because
    // the "enhanced" one binds `descriptorCount` to the number of immutable samplers,
    // which leads to 0, which is wrong.
    vk::DescriptorSetLayoutBinding uboLayoutBinding{
        0, vk::DescriptorType::eUniformBuffer, 1, shaderStageFlags, {}};

    vk::DescriptorSetLayoutCreateInfo layoutInfo{{}, {uboLayoutBinding}};

    m_descriptorSetLayout = m_device->createDescriptorSetLayoutUnique(layoutInfo);
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

    vk::PipelineRasterizationStateCreateInfo rasterizationCreateInfo{
        {},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eCounterClockwise,
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

    vk::PipelineLayoutCreateInfo layoutCreateInfo{{}, {m_descriptorSetLayout.get()}};

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

void HelloTriangleApplication::CreateTextureImage(UploadQueue &uploadQueue)
{
    int width = 0, height = 0, channels = 0;
    std::filesystem::path texturePath = m_texturePath / "texture.jpg";

#ifdef _WIN32
    std::string filepath = WideCharToUTF8(texturePath);
    const char *pFilePath = filepath.c_str();
#else
    const char *pFilePath = texturePath.c_str();
#endif

    stbi_uc *pixels = stbi_load(pFilePath, &width, &height, &channels, STBI_rgb_alpha);

    VmaImage *pImage = uploadQueue.UploadImage(width, height, width * height * 4, pixels);
    m_textureImage.reset(pImage);

    stbi_image_free(pixels);
}

void HelloTriangleApplication::CreateVertexBuffer(UploadQueue &uploadQueue)
{
    std::vector<Vertex> vs = Vertices;
    const std::span<Vertex> vertices{vs};
    std::vector<uint16_t> is = Indices;
    const std::span<uint16_t> indices{is};

    VmaBuffer *vertexBuffer =
        uploadQueue.UploadData(vertices, vk::BufferUsageFlagBits::eVertexBuffer);
    m_vertexBuffer.reset(vertexBuffer);

    VmaBuffer *indexBuffer = uploadQueue.UploadData(indices, vk::BufferUsageFlagBits::eIndexBuffer);
    m_indexBuffer.reset(indexBuffer);
}

void HelloTriangleApplication::CreateFrameData()
{
    for (auto i = 0; i < m_frameDatas.size(); ++i)
    {
        m_frameDatas[i].emplace(m_device.get(), m_vma.Allocator(), m_commandPool.get());
    }
}

void HelloTriangleApplication::CreateDescriptorPool()
{
    vk::DescriptorPoolSize poolSize{vk::DescriptorType::eUniformBuffer, MaxFramesInFlight};

    vk::DescriptorPoolCreateFlags flags{vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet};
    vk::DescriptorPoolCreateInfo poolInfo{flags, MaxFramesInFlight, {poolSize}};
    m_descriptorPool = m_device->createDescriptorPoolUnique(poolInfo);
}

void HelloTriangleApplication::CreateDescriptorSets()
{
    std::array<vk::DescriptorSetLayout, MaxFramesInFlight> layouts;
    for (vk::DescriptorSetLayout &layout : layouts)
    {
        layout = m_descriptorSetLayout.get();
    }

    vk::DescriptorSetAllocateInfo allocInfo{m_descriptorPool.get(), layouts};
    m_descriptorSets = m_device->allocateDescriptorSetsUnique(allocInfo);

    for (auto i = 0; i < m_descriptorSets.size(); ++i)
    {
        vk::DescriptorBufferInfo bufferInfo{m_frameDatas[i]->GetUniformBuffer(), 0,
                                            sizeof(UniformBufferObject)};
        vk::WriteDescriptorSet descriptorWrites{
            m_descriptorSets[i].get(), 0, 0, vk::DescriptorType::eUniformBuffer, {}, {bufferInfo}};

        m_device->updateDescriptorSets({descriptorWrites}, {});
    }
}

void HelloTriangleApplication::RecordCommandBuffer(FrameData<UniformBufferObject> &frameData,
                                                   uint32_t imageIndex)
{
    vk::CommandBuffer commandBuffer = frameData.CommandBuffer();

    vk::CommandBufferBeginInfo beginInfo{};
    commandBuffer.begin(beginInfo);

    UpdateUniformBuffer(frameData);

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

    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0,
                                     m_descriptorSets[m_currentFrame].get(), {});
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
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();
    CreateFrameBuffers();
    CreateCommandPool();

    UploadQueue uploadQueue{m_vma, m_device.get(), m_familyIndices->GraphicsFamily().value()};

    CreateTextureImage(uploadQueue);
    CreateVertexBuffer(uploadQueue);

    uploadQueue.FinishAndWait();

    CreateFrameData();
    CreateDescriptorPool();
    CreateDescriptorSets();
}

void HelloTriangleApplication::MainLoop()
{
    m_window->Loop([this]() { this->DrawFrame(); });
    m_device->waitIdle();
}

void HelloTriangleApplication::UpdateUniformBuffer(FrameData<UniformBufferObject> &frameData)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time =
        std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    glm::mat4 model =
        glm::rotate(glm::mat4(1.f), time * glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 view =
        glm::lookAt(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 projection = glm::perspective(glm::radians(45.f), m_swapchain->Ratio(), 0.1f, 10.f);

    projection[1][1] *= -1;

    UniformBufferObject ubo{model, view, projection};
    frameData.UpdateUniform(ubo);
}

void HelloTriangleApplication::DrawFrame()
{
    auto &frameData = m_frameDatas[m_currentFrame].value();
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

    RecordCommandBuffer(frameData, imageIndex);

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