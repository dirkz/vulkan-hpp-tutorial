// vk-hpp-tutorial.cpp : Defines the entry point for the application.
//

#include "vk-hpp-tutorial.h"

using namespace std;
using namespace vk;

static glm::mat4 transform(glm::vec2 const &Orientation, glm::vec3 const &Translate, glm::vec3 const &Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    return Proj * View * Model;
}

int main()
{
    vector<ExtensionProperties> properties = enumerateInstanceExtensionProperties();
    for (const auto &property : properties)
    {
        cout << "*** extension" << property.extensionName << "\n";
    }

    return 0;
}
