#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <vec2.hpp>
#include <vec3.hpp>

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
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t texture;

        Mesh();

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, uint32_t texture);

        void draw() const;
        void initialize_mesh();

    private:
        bool initialized = false;

        VkDeviceMemory vertex_buffer;
    };
}
