# CMakeList.txt : CMake project for vulkan-hpp-tutorial, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.29)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("vulkan-hpp-tutorial"
	VERSION 0.1
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(_UNICODE UNICODE)

find_package(Vulkan REQUIRED COMPONENTS glslc)

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "The Vulkan SDK or a required component of it could not be found, have you set VULKAN_SDK correctly?")
else()
    message(STATUS "Vulkan: Version ${Vulkan_VERSION}")
	message(STATUS "glsl: ${Vulkan_GLSLC_EXECUTABLE}")
endif()

# vector maths
add_subdirectory(glm)

add_subdirectory(glfw)

# main sources
add_subdirectory(source)
