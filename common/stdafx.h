// Make GLM suitable for vulkan
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
