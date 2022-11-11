#pragma once

#include <stdexcept>
#include <cstdint>
#include <string>
#include <map>

#include <glad/gl.h>
#include <stb_image.h>

namespace image_registry
{
    // static std::map<std::string, uint32_t> loaded_textures;
    
    uint32_t load_texture(const std::string file_name);

    uint32_t get_texture(const std::string file_name);

    uint32_t get_or_load_texture(const std::string file_name);
};
