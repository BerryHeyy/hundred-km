#pragma once

#include <string>

std::string get_process_path();

const std::string RESOURCES_PATH = get_process_path() + "resources/";
const std::string TEXTURES_PATH = RESOURCES_PATH + "texture/";
const std::string OBJ_PATH = RESOURCES_PATH + "model/obj/";
const std::string MTL_PATH = OBJ_PATH;


