#include "swapchain.hpp"

#include <limits>
#include <algorithm>
#include <stdexcept>

namespace hkm
{

Swapchain::Swapchain() 
{

}

Swapchain::Swapchain(const LogicalDevice* device, const VkSurfaceKHR surface, VkExtent2D window_extent) : device {device}, surface {surface}
{
    create_swapchain(window_extent);
    create_image_views();
    create_render_pass();
    create_frame_buffers();
}

Swapchain::Swapchain(Swapchain&& source) : device {source.device}, surface {surface}
{
    move_from_source(std::move(source));
}

Swapchain::~Swapchain()
{
    if (device == nullptr) return;

    cleanup_swapchain();

    vkDestroyRenderPass(device->get_logical_device(), render_pass, nullptr);
}

void Swapchain::recreate_swapchain(VkExtent2D window_extent)
{
    cleanup_swapchain();

    create_swapchain(window_extent);
    create_image_views();
    create_frame_buffers();
}

void Swapchain::cleanup_swapchain()
{

    for (VkFramebuffer framebuffer : swapchain_framebuffers)
    {
        vkDestroyFramebuffer(device->get_logical_device(), framebuffer, nullptr);
    }

    for (VkImageView image_view : swapchain_image_views)
    {
        vkDestroyImageView(device->get_logical_device(), image_view, nullptr);
    }

    vkDestroySwapchainKHR(device->get_logical_device(), swapchain_handle, nullptr);
}

VkSwapchainKHR Swapchain::get_swapchain_handle() const
{
    return swapchain_handle;
}

VkExtent2D Swapchain::get_swapchain_extent() const
{
    return swapchain_extent;
}

VkRenderPass Swapchain::get_render_pass() const
{
    return render_pass;
}

std::vector<VkFramebuffer>& Swapchain::get_swapchain_framebuffers()
{
    return swapchain_framebuffers;
}

Swapchain& Swapchain::operator = (Swapchain&& source)
{
    if (device != nullptr) cleanup_swapchain();

    move_from_source(std::move(source));

    device = source.device;
    surface = source.surface;

    return *this;
}

void Swapchain::create_swapchain(VkExtent2D window_extent)
{
    SwapchainSupportDetails swap_chain_support_details = device->query_swap_chain_support(surface);

    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support_details.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support_details.present_modes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support_details.capabilites, window_extent);

    uint32_t swap_chain_image_count = swap_chain_support_details.capabilites.minImageCount + 1;

    // Make sure to not exceed the maximum number of images
    if (swap_chain_support_details.capabilites.maxImageCount > 0 && 
        swap_chain_image_count > swap_chain_support_details.capabilites.maxImageCount)
    {
        swap_chain_image_count = swap_chain_support_details.capabilites.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = swap_chain_image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = device->find_queue_families(surface);
    uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

    if (indices.graphics_family != indices.present_family)
    {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0; // Optional
        create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    create_info.preTransform = swap_chain_support_details.capabilites.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device->get_logical_device(), &create_info, nullptr, &swapchain_handle) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create swap chain!");
    }

    // Get swap chain image handles
    vkGetSwapchainImagesKHR(device->get_logical_device(), swapchain_handle, &swap_chain_image_count, nullptr);
    swapchain_images.resize(swap_chain_image_count);
    vkGetSwapchainImagesKHR(device->get_logical_device(), swapchain_handle, &swap_chain_image_count, swapchain_images.data());

    // Store data for future use
    swapchain_image_format = surface_format.format;
    swapchain_extent = extent;
}

void Swapchain::create_image_views()
{
    swapchain_image_views.resize(swapchain_images.size());

    for (size_t i = 0; i < swapchain_images.size(); i++)
    {
        VkImageViewCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swapchain_images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapchain_image_format;

        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device->get_logical_device(), &create_info, nullptr, &swapchain_image_views[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

void Swapchain::create_render_pass()
{
    VkAttachmentDescription color_attachment {};
    color_attachment.format = swapchain_image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;

    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(device->get_logical_device(), &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render pass!");
    }
}

void Swapchain::create_frame_buffers()
{
    swapchain_framebuffers.resize(swapchain_image_views.size());

    for (size_t i = 0; i < swapchain_image_views.size(); i++)
    {
        VkImageView attachments[] = {
            swapchain_image_views[i]
        };

        VkFramebufferCreateInfo framebuffer_info {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swapchain_extent.width;
        framebuffer_info.height = swapchain_extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(device->get_logical_device(), &framebuffer_info, nullptr, &swapchain_framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

// Swapchain helper functions

VkSurfaceFormatKHR Swapchain::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
    for (VkSurfaceFormatKHR available_format : available_formats)
    {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && 
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return available_format; // Return preferred format
        }
    }

    return available_formats[0]; // Return first format specified
}

VkPresentModeKHR Swapchain::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
    // Check if preferred format is available.
    for (VkPresentModeKHR available_present_mode : available_present_modes)
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return available_present_mode; // Return preferred present mode
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR; // Return guaranteed present mode
}

VkExtent2D Swapchain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D window_extent)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actual_extent = window_extent;

        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actual_extent;
    }
}

void Swapchain::move_from_source(Swapchain&& source)
{
    swapchain_handle = source.swapchain_handle;
    swapchain_image_format = source.swapchain_image_format;
    swapchain_extent = source.swapchain_extent;
    swapchain_images = source.swapchain_images;
    swapchain_image_views = source.swapchain_image_views;
    swapchain_framebuffers = source.swapchain_framebuffers;
    render_pass = source.render_pass;

    source.swapchain_handle = nullptr;
    source.swapchain_images.clear();
    source.swapchain_image_views.clear();
    source.swapchain_framebuffers.clear();

    source.render_pass = nullptr;
}

}
