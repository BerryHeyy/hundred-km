#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace hkm
{

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
};

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilites;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

class LogicalDevice
{
public:

    LogicalDevice();
    LogicalDevice(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers, VkSurfaceKHR surface);

    LogicalDevice(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers, VkPhysicalDevice physical_device, QueueFamilyIndices indices);

    LogicalDevice(LogicalDevice&& source);

    LogicalDevice(const LogicalDevice&) = delete; // Explicity disable copies as this would mess up vulkan handlers

    ~LogicalDevice();

    LogicalDevice& operator = (LogicalDevice&& source);

    LogicalDevice& operator = (const LogicalDevice&) = delete; // Explicity disable copies as this would mess up vulkan handlers

    VkDevice get_logical_device() const;
    VkQueue get_present_queue() const;
    VkQueue get_graphics_queue() const;

    SwapchainSupportDetails query_swap_chain_support(VkSurfaceKHR surface) const;
    QueueFamilyIndices find_queue_families(VkSurfaceKHR surface) const;

private:
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    VkQueue graphics_queue, present_queue;

    void pick_physical_device(const std::vector<const char*>& device_extensions, VkSurfaceKHR surface);
    void create_logical_device(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers, QueueFamilyIndices indices);

    // Helper functions
    bool check_device_extension_support(VkPhysicalDevice device, const std::vector<const char*>& device_extensions);
    SwapchainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    bool is_device_suitable(VkPhysicalDevice device, const std::vector<const char*>& device_extensions, VkSurfaceKHR surface);

    // Move
    void move_from_source(LogicalDevice&& source);
};

}
