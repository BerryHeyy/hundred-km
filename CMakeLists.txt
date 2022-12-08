cmake_minimum_required (VERSION 3.14)

project (hundred-km)

add_subdirectory (dependencies/glfw/)

set (HKM_SOURCES
    hundred-km.cpp
    vulkan_handler.hpp
    vulkan_handler.cpp
    resource_handler.hpp
    resource_handler.cpp
)
list (TRANSFORM HKM_SOURCES PREPEND "src/")

add_executable (${PROJECT_NAME} ${HKM_SOURCES})
target_include_directories (${PROJECT_NAME} PRIVATE dependencies/glad/include dependencies/glfw/include/ dependencies/glm/glm dependencies/stb/ dependencies/OBJ-Loader/Source)
target_link_libraries (${PROJECT_NAME} glfw)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries (${PROJECT_NAME} -static-libgcc -static-libstdc++ -static)
endif (CMAKE_BUILD_TYPE MATCHES "Release")

if (WIN32)

    add_compile_definitions(ISWIN)

    message(STATUS "Generating build files specifically for windows.")

    message(STATUS "Attempting to locate Vulkan SDK using manual path...")
	set(VULKAN_SDK "C:/VulkanSDK")
	set(VULKAN_VERSION "1.3.231.1")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using manually specified path: ${VULKAN_PATH}")

    # Check if manually set path exists
	if(NOT EXISTS "${VULKAN_PATH}")
        message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.")
        return()
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${VULKAN_PATH}/Include)
    target_link_libraries(${PROJECT_NAME} ${VULKAN_PATH}/Lib/vulkan-1.lib)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/resources")
        message(STATUS "Creating symlink for resources.")

        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/resources" SYM_SHADER_LINK)
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/resources" SYM_SHADER_TARGET)
    
        execute_process(
            COMMAND cmd.exe /c mklink /J "${SYM_SHADER_LINK}" "${SYM_SHADER_TARGET}"
        )
    endif()

endif(WIN32)
if (UNIX)

    add_compile_definitions(ISLINUX)

    message(STATUS "Generating build files specifically for linux.")

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/resources")
        message(STATUS "Creating symlink for resources.")

        target_link_libraries(${PROJECT_NAME} vulkan)

        execute_process(
            COMMAND ln -s ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
        )
    endif()

endif (UNIX)
