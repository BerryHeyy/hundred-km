#include "image_registry.hpp"

static std::map<std::string, uint32_t> loaded_textures;

uint32_t image_registry::load_texture(const std::string file_name)
{
    if (loaded_textures.count(file_name))
    {
        throw std::runtime_error("Tried to load texture `" + file_name + "`, which already exists.");
    }

    int width, height, nr_channels;
    unsigned char *data = stbi_load(("resources\\texture\\" + file_name).c_str(), &width, &height, &nr_channels, 0); 

    if (data == NULL)
    {
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture `" + file_name + "`.");
    }

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

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
