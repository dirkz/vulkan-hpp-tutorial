#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <comdef.h>
#include <windows.h>

#endif

// Make GLM suitable for vulkan
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
