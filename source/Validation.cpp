#include "Validation.h"

#include "Constants.h"

using namespace std;

namespace zvk
{

static const std::vector<const char *> ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

static bool CheckValidationLayerSupport()
{
    vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();

    set<string> layerNames{};

    for (const vk::LayerProperties &properties : layerProperties)
    {
        layerNames.insert(properties.layerName);
    }

    for (const char *validationLayer : ValidationLayers)
    {
        string validationLayerName{validationLayer};
        auto result = find(layerNames.begin(), layerNames.end(), validationLayerName);
        if (result == layerNames.end())
        {
            return false;
        }
    }

    return true;
}

std::vector<const char *> EnabledValidationLayers()
{
    if (EnableValidationLayers && CheckValidationLayerSupport())
    {
        return ValidationLayers;
    }
    else
    {
        if (EnableValidationLayers && !CheckValidationLayerSupport())
        {
            throw std::runtime_error{
                "validation layers requested, but none defined by the instance"};
        }
        return {};
    }
}

} // namespace zvk