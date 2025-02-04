#include "QueueFamilyIndices.h"

using namespace std;

namespace zvk
{

QueueFamilyIndices::QueueFamilyIndices(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
    vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            m_graphicsFamily = i;
        }

        VkBool32 surfaceSupport = device.getSurfaceSupportKHR(i, surface);

        if (surfaceSupport)
        {
            m_presentFamily = i;
        }

        if (IsComplete())
        {
            break;
        }
    }
}

bool QueueFamilyIndices::IsComplete()
{
    return m_graphicsFamily.has_value() && m_presentFamily.has_value();
}

std::optional<uint32_t> QueueFamilyIndices::GraphicsFamily()
{
    return m_graphicsFamily;
}

std::optional<uint32_t> QueueFamilyIndices::PresentFamily()
{
    return m_presentFamily;
}

std::vector<uint32_t> QueueFamilyIndices::UniqueFamilies()
{
    set<uint32_t> familySet{m_graphicsFamily.value(), m_presentFamily.value()};
    return vector<uint32_t>{familySet.begin(), familySet.end()};
}

} // namespace zvk