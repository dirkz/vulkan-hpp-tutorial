#include "QueueFamilyIndices.h"

using namespace std;

namespace zvk
{

QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice device,
                                       const vk::SurfaceKHR surface)
{
    vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        if (!m_graphicsFamily.has_value())
        {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_graphicsFamily = i;
            }
        }

        if (!m_transferFamily.has_value())
        {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eTransfer &&
                !(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics))
            {
                m_transferFamily = i;
            }
        }

        if (!m_presentFamily.has_value())
        {
            VkBool32 surfaceSupport = device.getSurfaceSupportKHR(i, surface);

            if (surfaceSupport)
            {
                m_presentFamily = i;
            }
        }

        if (IsComplete())
        {
            break;
        }
    }
}

bool QueueFamilyIndices::IsComplete() const
{
    return m_graphicsFamily.has_value() && m_presentFamily.has_value() &&
           m_transferFamily.has_value();
}

std::optional<uint32_t> QueueFamilyIndices::GraphicsFamily() const
{
    return m_graphicsFamily;
}

std::optional<uint32_t> QueueFamilyIndices::PresentFamily() const
{
    return m_presentFamily;
}

std::optional<uint32_t> QueueFamilyIndices::TransferFamily() const
{
    return m_transferFamily;
}

std::vector<uint32_t> QueueFamilyIndices::UniqueFamilies() const
{
    set<uint32_t> familySet{m_graphicsFamily.value(), m_presentFamily.value(),
                            m_transferFamily.value()};
    return vector<uint32_t>{familySet.begin(), familySet.end()};
}

std::vector<uint32_t> QueueFamilyIndices::UniqueGraphicsAndPresent() const
{
    set<uint32_t> familySet{m_graphicsFamily.value(), m_presentFamily.value()};
    return vector<uint32_t>{familySet.begin(), familySet.end()};
}

std::vector<uint32_t> QueueFamilyIndices::UniqueGraphicsAndTransfer() const
{
    set<uint32_t> familySet{m_graphicsFamily.value(), m_transferFamily.value()};
    return vector<uint32_t>{familySet.begin(), familySet.end()};
}

} // namespace zvk