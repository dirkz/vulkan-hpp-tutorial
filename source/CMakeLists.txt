# CMakeList.txt : CMake project for vk-hpp-tutorial, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.29)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# precompiled header
add_subdirectory(common)

# Add source to this project's executable.
add_executable(${PROJECT_NAME}
	"main.cpp"
	"HelloTriangleApplication.h"
	"HelloTriangleApplication.cpp"
	"Constants.h"
	"Validation.h"
	"Validation.cpp"
	"Extensions.h"
	"Extensions.cpp"
	"DebugUtilsMessenger.h"
	"DebugUtilsMessenger.cpp"
	"Strings.h"
	"Strings.cpp"
	"QueueFamilyIndices.h"
	"QueueFamilyIndices.cpp"
	"SwapChainSupportDetails.h"
	"SwapChainSupportDetails.cpp"
	"Window.h"
	"Window.cpp"
	"SwapChain.h"
	"SwapChain.cpp")

# Adds shader compilations as custom target to ${PROJECT_NAME}
add_subdirectory(shaders)

target_link_libraries(${PROJECT_NAME} PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

