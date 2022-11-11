#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

class Shader
{
public:
    
    uint32_t id;

    Shader(const char* vertex_path, const char* fragment_path);

    void use() const;

    void set_bool(const std::string &name, bool value) const;  
    void set_int(const std::string &name, int value) const;   
    void set_float(const std::string &name, float value) const;
};
