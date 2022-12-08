#pragma once

#include <GLFW/glfw3.h>

namespace hkm::vulkan_handler
{
    void init_vulkan();

    void draw_frame();

    void wait_for_exit();
    void cleanup_vulkan();
}

