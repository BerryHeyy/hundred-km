#include "path_helper.hpp"

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
            throw std::runtime_error("Could not find process path.");

        buff[len] = '\0';

        path = std::string(buff);

        size_t spost = path.rfind('/');
        
        if (spost == std::string::npos)
            throw std::runtime_error("Could not find process path.");

        path = path.substr(0, spost + 1);
#endif
#ifdef ISWIN
        
#endif
        
    }

    return path;
}