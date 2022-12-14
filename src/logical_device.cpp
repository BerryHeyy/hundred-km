#include "logical_device.hpp"

#include <set>
#include <stdexcept>

#include "swapchain.hpp"

extern VkInstance vk_instance;

namespace hkm
{

LogicalDevice::LogicalDevice()
{

}

LogicalDevice::LogicalDevice(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers, VkSurfaceKHR surface)
{
    pick_physical_device(device_extensions, surface);

    queue_indices = find_queue_families(surface);

    create_logical_device(device_extensions, validation_layers);
    create_command_pool();
}

LogicalDevice::LogicalDevice(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers, VkPhysicalDevice physical_device, QueueFamilyIndices indices) 
    : physical_device {physical_device}, queue_indices {indices}
{
    create_logical_device(device_extensions, validation_layers);
    create_command_pool();
}

LogicalDevice::LogicalDevice(LogicalDevice&& source)
{
    move_from_source(std::move(source));
}

LogicalDevice::~LogicalDevice() {}

LogicalDevice& LogicalDevice::operator = (LogicalDevice&& source)
{
    if (logical_device != nullptr) destroy();

    move_from_source(std::move(source));
    
    return *this;
}

void LogicalDevice::destroy()
{
    vkDestroyCommandPool(logical_device, command_pool, nullptr);

    vkDestroyDevice(logical_device, nullptr);
}

VkDevice LogicalDevice::get_logical_device() const
{
    return logical_device;
}

VkQueue LogicalDevice::get_present_queue() const
{
    return present_queue;
}

VkQueue LogicalDevice::get_graphics_queue() const
{
    return graphics_queue;
}

VkCommandPool LogicalDevice::get_command_pool() const
{
    return command_pool;
}

VkBuffer LogicalDevice::create_buffer(VkBufferCreateInfo buffer_info) const
{
    VkBuffer buffer;

    if (vkCreateBuffer(logical_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    return buffer;
}

VkDeviceMemory LogicalDevice::allocate_memory(VkMemoryAllocateInfo allocation_info) const
{
    VkDeviceMemory memory;

    if (vkAllocateMemory(logical_device, &allocation_info, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    return memory;
}

void LogicalDevice::pick_physical_device(const std::vector<const char*>& device_extensions, VkSurfaceKHR surface)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

    if (device_count == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

    for (VkPhysicalDevice device : devices)
    {
        if (is_device_suitable(device, device_extensions, surface))
        {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable GPU.");
    }
}

void LogicalDevice::create_logical_device(const std::vector<const char*>& device_extensions, const std::vector<const char*>& validation_layers)
{

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = {queue_indices.graphics_family.value(), queue_indices.present_family.value()};

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    // Specify features
    VkPhysicalDeviceFeatures device_features {};

    // Create device
    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();

    create_info.pEnabledFeatures = &device_features;

    // Enable device extensions
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

#ifdef NDEBUG
    create_info.enabledLayerCount = 0;
#else
    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames = validation_layers.data();
#endif


    if (vkCreateDevice(physical_device, &create_info, nullptr, &logical_device) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device.");
    }

    vkGetDeviceQueue(logical_device, queue_indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(logical_device, queue_indices.present_family.value(), 0, &present_queue);
}

bool LogicalDevice::check_device_extension_support(VkPhysicalDevice device, const std::vector<const char*>& device_extensions)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    for (VkExtensionProperties extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

void LogicalDevice::create_command_pool()
{
    VkCommandPoolCreateInfo pool_info {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_indices.graphics_family.value();

    if (vkCreateCommandPool(logical_device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create command pool!");
    }
}

QueueFamilyIndices LogicalDevice::find_queue_families(VkSurfaceKHR surface) const
{
    return find_queue_families(physical_device, surface);
}

QueueFamilyIndices LogicalDevice::find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    // Check if at least one queue family exists that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (VkQueueFamilyProperties queue_family : queue_families) //TODO: implement an early exit.
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) 
        {
            indices.present_family = i;
        }

        i++;
    }

    return indices;
}

bool LogicalDevice::is_device_suitable(VkPhysicalDevice device, const std::vector<const char*>& device_extensions, VkSurfaceKHR surface)
{
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    // Get the queue indices
    QueueFamilyIndices indices = find_queue_families(device, surface);

    // Check if the required extensions are supported by the physical device (gpu)
    bool extensions_supported = check_device_extension_support(device, device_extensions);

    // Check if swapchain supported by the physcial device is adequate for our needs
    bool swap_chain_adequate = false;
    if (extensions_supported)
    {
        SwapchainSupportDetails swap_chain_support_details = query_swap_chain_support(device, surface);

        swap_chain_adequate = !swap_chain_support_details.formats.empty() &&
            !swap_chain_support_details.present_modes.empty();
    }

    return extensions_supported && // Gpu needs to support extensions
        swap_chain_adequate &&
        indices.graphics_family.has_value() && // Gpu needs to have graphics queue family.
        indices.present_family.has_value(); // Gpu needs to have present queue family.
}

SwapchainSupportDetails LogicalDevice::query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
    SwapchainSupportDetails details;

    // Query basic capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilites);

    // Query supported surface formats
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

    if (format_count != 0)
    {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
    }

    // Query supported presentation modes
    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0)
    {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
    }

    return details;
}

SwapchainSupportDetails LogicalDevice::query_swap_chain_support(VkSurfaceKHR surface) const
{
    return query_swap_chain_support(physical_device, surface);
}

void LogicalDevice::move_from_source(LogicalDevice&& source)
{
    physical_device = source.physical_device;
    logical_device = source.logical_device;
    graphics_queue = source.graphics_queue;
    present_queue = source.present_queue;
    command_pool = source.command_pool;

    source.physical_device = nullptr;
    source.logical_device = nullptr;
    source.graphics_queue = nullptr;
    source.present_queue = nullptr;
    source.command_pool = nullptr;
}

}
