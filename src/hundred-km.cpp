#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_handler.hpp"

int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

GLFWwindow* glfw_window;

void init_window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfw_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hundred Kilometers", nullptr, nullptr);
}

void cleanup()
{
    hkm::vulkan_handler::cleanup_vulkan();

    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

int main()
{
    init_window();
    hkm::vulkan_handler::init_vulkan();

    // Game loop
    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwPollEvents();

        hkm::vulkan_handler::draw_frame();
    }

    hkm::vulkan_handler::wait_for_exit();

    cleanup();

    return 0;
}