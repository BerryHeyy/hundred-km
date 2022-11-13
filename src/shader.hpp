#pragma once

#include <cstdint>
#include <string>

#include <mat4x4.hpp>

class Shader
{
public:
    
    uint32_t id;

    Shader(const char* vertex_path, const char* fragment_path);

    void use() const;

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, const glm::mat4 &value) const;
};
