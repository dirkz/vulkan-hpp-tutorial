#include "Util.h"

namespace vkz
{

std::vector<const char *> CharPointers(std::vector<std::string> strings)
{
    std::vector<const char *> result;

    std::transform(strings.begin(), strings.end(), std::back_inserter(result),
                   [](const std::string &s) { return s.c_str(); });

    return result;
}

} // namespace vkz
