#include "shader.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    // ensure ifstream objects can throw exceptions:
    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_file.open(vertex_path);
        vertex_shader_stream << vertex_shader_file.rdbuf();
        vertex_shader_file.close();
        vertex_code = vertex_shader_stream.str();

        fragment_shader_file.open(fragment_path);
        fragment_shader_stream << fragment_shader_file.rdbuf();
        fragment_shader_file.close();
        fragment_code = fragment_shader_stream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR READING SHADER FILE\n" << e.what() << std::endl;
    }

    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "ERROR VERTEX SHADER COMPILATION\n" << info_log << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "ERROR FRAGMENT SHADER COMPILATION\n" << info_log << std::endl;
        std::cout << fragment_shader_code << std::endl;
    }

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        std::cout << "ERROR PROGRAM COMPILATION\n" << info_log << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(program_id);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);

}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program_id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    Shader::setVec3(name, v.x, v.y, v.z);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string& name, const glm::vec4& v) const {
    Shader::setVec4(name, v.x, v.y, v.z, v.w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}