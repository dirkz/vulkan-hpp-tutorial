#include "Validation.h"

using namespace std;

namespace zvk
{

static const std::vector<const char *> ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
static const bool EnableValidationLayers = false;
#else
static const bool EnableValidationLayers = true;
#endif

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
        return {};
    }
}

} // namespace zvk