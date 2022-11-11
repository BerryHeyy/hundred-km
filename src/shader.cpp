#include "shader.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string v_code;
    std::string f_code;
    std::ifstream v_file;
    std::ifstream f_file;

    // Ensure ifstream objects can throw exceptions
    v_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        v_file.open(vertex_path);
        f_file.open(fragment_path);
        std::stringstream ss;

        ss << v_file.rdbuf();
        v_code = ss.str();

        ss.str(std::string()); // Clear string stream

        ss << f_file.rdbuf();
        f_code = ss.str();

        v_file.close();
        f_file.close();
    }
    catch (std::ifstream::failure e)
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* v_code_c = v_code.c_str();
    const char* f_code_c = f_code.c_str();

    // Compile shaders
    uint32_t v_id, f_id;
    int success;
    char info_log[512];

    v_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_id, 1, &v_code_c, NULL);
    glCompileShader(v_id);

    glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(v_id, 512, NULL, info_log);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(info_log));
    };

    f_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_id, 1, &f_code_c, NULL);
    glCompileShader(f_id);

    glGetShaderiv(f_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(f_id, 512, NULL, info_log);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(info_log));
    };

    this->id = glCreateProgram();
    glAttachShader(this->id, v_id);
    glAttachShader(this->id, f_id);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(info_log));
    }

    glDeleteShader(v_id);
    glDeleteShader(f_id);
}

void Shader::use() const
{
    glUseProgram(this->id);
}

void Shader::set_bool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value); 
}
void Shader::set_int(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value); 
}
void Shader::set_float(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value); 
} 
