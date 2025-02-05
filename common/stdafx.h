// Make GLM suitable for vulkan
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>

#endif

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <set>
#include <optional>
#include <limits>
#include <algorithm>
