set (SHADERS shader.frag shader.vert)

add_custom_target(shaders)

foreach(SHADER ${SHADERS})
	set (SHADER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
	set (SHADER_OUTPUT_FILENAME ${SHADER}.spv)
	set (SHADER_TARGET ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OUTPUT_FILENAME})

	IF(CMAKE_BUILD_TYPE MATCHES Debug)
		# something?
	ENDIF()

	add_custom_command(
		OUTPUT ${SHADER_TARGET}
		DEPENDS ${SHADER_SRC}
		COMMAND ${Vulkan_GLSLC_EXECUTABLE} ARGS ${SHADER_SRC} -o ${SHADER_TARGET}
	)

	target_sources(shaders PRIVATE ${SHADER_TARGET})
endforeach()

add_dependencies(${PROJECT_NAME} shaders)