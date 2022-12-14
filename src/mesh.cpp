#include "mesh.hpp"

namespace hkm
{

Mesh::Mesh()
{
    
}

Mesh::Mesh(LogicalDevice* ld, std::vector<Vertex> vertices, std::vector<unsigned int> indices, uint32_t texture)
    : device {ld}
{
    // Request buffer and memory and bind
    VkBufferCreateInfo buffer_info {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = sizeof(Vertex) * vertices.size();
    buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vertex_buffer = device->create_buffer(buffer_info);

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(device->get_logical_device(), vertex_buffer, &mem_requirements);

    VkMemoryAllocateInfo allocate_info {};
    allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info.allocationSize = mem_requirements.size;
    allocate_info.memoryTypeIndex = device->find_memory_type(mem_requirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vertex_buffer_memory = device->allocate_memory(allocate_info);

    vkBindBufferMemory(device->get_logical_device(), vertex_buffer, vertex_buffer_memory, 0);
    
    initialize_mesh(vertices, indices, texture);
}

Mesh::~Mesh()
{
    vkDestroyBuffer(device->get_logical_device(), vertex_buffer, nullptr);
}

void Mesh::initialize_mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, uint32_t texture)
{

}

}


