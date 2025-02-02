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

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
