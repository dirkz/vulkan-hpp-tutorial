#include "QueueFamilyIndices.h"

using namespace std;

namespace zvk
{

QueueFamilyIndices::QueueFamilyIndices(vk::PhysicalDevice device)
{
    vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    for (int i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            m_graphicsFamily = i;
        }

        if (IsComplete())
        {
            break;
        }
    }
}

bool QueueFamilyIndices::IsComplete()
{
    return m_graphicsFamily.has_value();
}

std::optional<uint32_t> QueueFamilyIndices::GraphicsFamiliy()
{
    return m_graphicsFamily;
}

} // namespace zvk