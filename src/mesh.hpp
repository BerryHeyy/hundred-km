#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <vec2.hpp>
#include <vec3.hpp>

#include "logical_device.hpp"

namespace hkm
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

    class Mesh
    {
    public:
        Mesh();
        Mesh(LogicalDevice* ld, std::vector<Vertex> vertices, std::vector<unsigned int> indices, uint32_t texture);
        ~Mesh();

        void draw() const;
        void initialize_mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, uint32_t texture);

    private:
        LogicalDevice* device;
        bool initialized = false;

        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_buffer_memory;
    };
}
