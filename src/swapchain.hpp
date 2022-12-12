#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "logical_device.hpp"

namespace hkm
{

class Swapchain
{
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    Swapchain();
    Swapchain(const LogicalDevice* device, const VkSurfaceKHR surface, VkExtent2D window_extent);

    Swapchain(Swapchain&& source);

    Swapchain(const Swapchain&) = delete; // Explicity disable copies as this would mess up vulkan handlers

    ~Swapchain();

    Swapchain& operator = (Swapchain&& source);

    Swapchain& operator = (const Swapchain&) = delete; // Explicity disable copies as this would mess up vulkan handlers

    void recreate_swapchain(VkExtent2D window_extent);
    void cleanup_swapchain();

    VkSwapchainKHR get_swapchain_handle() const;
    VkExtent2D get_swapchain_extent() const;
    VkRenderPass get_render_pass() const;

    std::vector<VkFramebuffer>& get_swapchain_framebuffers();

    void destroy();

private:
    VkSwapchainKHR swapchain_handle;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_extent;
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;
    std::vector<VkFramebuffer> swapchain_framebuffers;

    VkRenderPass render_pass;

    const LogicalDevice* device = nullptr;
    VkSurfaceKHR surface = nullptr;

    void create_swapchain(VkExtent2D window_extent);
    void create_image_views();
    void create_render_pass();
    void create_frame_buffers();
    void create_sync_objects();

    // Helper functions
    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D window_extent);

    // Move
    void move_from_source(Swapchain&& source);
};

}
