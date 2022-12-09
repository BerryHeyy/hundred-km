#pragma once

#include <vector>
#include <string>

namespace hkm::resource_handler
{
    std::string get_working_directory();

    std::vector<char> read_file_binary(const std::string& filename);
    std::vector<char> read_shader_binary(const std::string& shader_name);
}
