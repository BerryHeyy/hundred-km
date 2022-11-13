cmake_minimum_required (VERSION 3.14)

project (hundred-km)

add_subdirectory (dependencies/glfw/)

set (HKM_SOURCES
    hundred-km.cpp
    mesh.hpp
    mesh.cpp
    shader.hpp
    shader.cpp
    player.hpp
    player.cpp
    model.hpp
    model.cpp
    image_registry.hpp
    image_registry.cpp
    path_helper.hpp
    path_helper.cpp
)
list (TRANSFORM HKM_SOURCES PREPEND "src/")

add_executable (${PROJECT_NAME} ${HKM_SOURCES} dependencies/glad/src/gl.c)
target_include_directories (${PROJECT_NAME} PRIVATE dependencies/glad/include dependencies/glfw/include/ dependencies/glm/glm dependencies/stb/ dependencies/OBJ-Loader/Source)
target_link_libraries (${PROJECT_NAME} glfw)

if (WIN32)

    add_compile_definitions(ISWIN)

    message(STATUS "Generating build files specifically for windows.")

    target_link_libraries(${PROJECT_NAME} opengl32.lib)

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

    target_link_libraries(${PROJECT_NAME} GL)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/resources")
        message(STATUS "Creating symlink for resources.")

        execute_process(
            COMMAND ln -s ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
        )
    endif()

endif (UNIX)
