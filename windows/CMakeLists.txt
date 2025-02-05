add_library(windows OBJECT
    Window.h
    GlfwWindow.h
    GlfwWindow.cpp)

target_sources(windows PUBLIC
  FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES Window.h GlfwWindow.h
 )

target_link_libraries(windows PRIVATE common)
target_link_libraries(windows PRIVATE glm::glm)
target_link_libraries(windows PRIVATE glfw)
target_link_libraries(windows PRIVATE Vulkan::Vulkan)
