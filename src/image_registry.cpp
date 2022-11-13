#include "image_registry.hpp"

#include <stdexcept>
#include <map>

#include <glad/gl.h>
#include <stb_image.h>

#include "path_helper.hpp"

static std::map<std::string, uint32_t> loaded_textures;

uint32_t image_registry::load_texture(const std::string file_name)
{
    if (loaded_textures.count(file_name))
    {
        throw std::runtime_error("Tried to load texture `" + file_name + "`, which already exists.");
    }

    int width, height, nr_channels;
    unsigned char *data = stbi_load((get_process_path() + "resources/texture/" + file_name).c_str(), &width, &height, &nr_channels, STBI_rgb_alpha); 

    if (data == NULL)
    {
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture `" + file_name + "`.");
    }

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    loaded_textures[file_name] = texture;

    stbi_image_free(data);

    return texture;
}

uint32_t image_registry::get_texture(const std::string file_name)
{
    if (!loaded_textures.count(file_name))
    {
        throw std::runtime_error("Tried to load texture `" + file_name + "`, which hasn't been loaded yet.");
    }

    return loaded_textures[file_name];
}

uint32_t image_registry::get_or_load_texture(const std::string file_name)
{
    if (!loaded_textures.count(file_name))
    {
        return load_texture(file_name);
    }
    else
    {
        return get_texture(file_name);
    }
}
