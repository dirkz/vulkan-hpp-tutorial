// vk-hpp-tutorial.cpp : Defines the entry point for the application.
//

#include "vk-hpp-tutorial.h"

using namespace std;
using namespace vk;

int main()
{
    vector<ExtensionProperties> properties = enumerateInstanceExtensionProperties();
    for (const auto &property : properties)
    {
        cout << "*** extension" << property.extensionName << "\n";
    }

    return 0;
}
