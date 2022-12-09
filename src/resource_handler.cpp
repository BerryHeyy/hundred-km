#include "resource_handler.hpp"

#include <fstream>

#ifdef ISLINUX
#include <unistd.h>
#endif
#ifdef ISWIN
#include <libloaderapi.h>
#endif

std::string get_process_path()
{
    static std::string path;

    if (path.empty())
    {
#ifdef ISLINUX
        char buff[1024];
        uint64_t len = readlink("/proc/self/exe", buff, 1023);

        if (len == -1)
        {
            throw std::runtime_error("Could not find process path.");
        }

        buff[len] = '\0';

        path = std::string(buff);

        size_t spos = path.rfind('/');
        
        if (spos == std::string::npos)
        {
            throw std::runtime_error("Could not find process path.");
        }

        path = path.substr(0, spos + 1);
#endif
#ifdef ISWIN
        char buff[MAX_PATH];
        uint32_t len = GetModuleFileName(NULL, buff, MAX_PATH);

        if (len == 0)
        {
            throw std::runtime_error("Could not find process path.");
        }

        path = std::string(buff);

        size_t spos = path.rfind('\\');

        path = path.substr(0, spos + 1);  
#endif
    }

    return path;
}

static const std::string RESOURCE_PATH  = get_process_path() + "resources/";
static const std::string TEXTURE_PATH = RESOURCE_PATH + "texture/";
static const std::string SHADER_PATH = RESOURCE_PATH + "shader/";
static const std::string OBJ_PATH = RESOURCE_PATH + "model/obj/";

static const std::string SHADER_EXTENSION = ".spv";

std::vector<char> hkm::resource_handler::read_file_binary(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error("Failed to open file " + filename);

    size_t file_size = (size_t) file.tellg();
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();
    return buffer;
}

std::vector<char> hkm::resource_handler::read_shader_binary(const std::string& shader_name)
{
    return read_file_binary(SHADER_PATH + shader_name + SHADER_EXTENSION);
}




