add_library(common INTERFACE)

target_include_directories(common INTERFACE ${Vulkan_INCLUDE_DIRS})

target_sources(common PUBLIC
  FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES stdafx.h
 )

target_precompile_headers(common INTERFACE stdafx.h)
