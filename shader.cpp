#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <gl/glew.h>

unsigned int Shader::compile(const char* path, unsigned int type)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku shadera: " << path << std::endl;
        return 0;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    std::string src = ss.str();
    const char* c_src = src.c_str();

    unsigned int shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &c_src, nullptr);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
        std::cerr << "B³¹d kompilacji shadera: " << path << "\n" << infoLog << std::endl;
    }
    return shader_id;
}

Shader::Shader(const char* vertPath, const char* fragPath)
{
    unsigned int vertexShader = compile(vertPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compile(fragPath, GL_FRAGMENT_SHADER);

    id_ = glCreateProgram();
    glAttachShader(id_, vertexShader);
    glAttachShader(id_, fragmentShader);
    glLinkProgram(id_);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void Shader::use() const { glUseProgram(id_); }
void Shader::setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(id_, name.c_str()), value); }
void Shader::setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value); }
void Shader::setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(id_, name.c_str()), value); }
void Shader::setVec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]); }
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]); }


void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}