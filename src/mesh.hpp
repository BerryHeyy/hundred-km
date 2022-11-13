#pragma once

#include <cstdint>
#include <vector>

#include <vec3.hpp>
#include <vec2.hpp>

#include "shader.hpp"

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

    void draw(Shader &shader) const;
    void initialize_mesh();

private:
    bool initialized = false;

    uint32_t VAO, VBO, EBO;
};
