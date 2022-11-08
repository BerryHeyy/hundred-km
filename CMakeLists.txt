cmake_minimum_required (VERSION 3.14)

project (hundred-km)

add_subdirectory (dependencies/glfw/)

set (HKM_SOURCES
    hundred-km.cpp
)
list (TRANSFORM HKM_SOURCES PREPEND "src/")

add_executable (${PROJECT_NAME} ${HKM_SOURCES} dependencies/glad/src/gl.c)
target_include_directories (${PROJECT_NAME} PRIVATE dependencies/glad/include dependencies/glfw/include/)
target_link_libraries (${PROJECT_NAME} glfw)

if (WIN32)

    message(STATUS "Generating build files specifically for windows.")

    target_link_libraries(${PROJECT_NAME} opengl32.lib)

endif(WIN32)
