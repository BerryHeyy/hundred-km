#pragma once

#include <vector>
#include <string>

namespace hkm::resource_handler
{
    std::vector<char> read_file(const std::string& filename);
}
