#include "path_helper.hpp"

#include <stdexcept>
#include <cstdint>

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