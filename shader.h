#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
private:
    unsigned int id_;
    static unsigned int compile(const char* path, unsigned int type);

public:
    Shader(const char* vertPath, const char* fragPath);
    void use() const;

    void setInt(const std::string& name, int value) const;
    void setBool(const std::string& name, bool value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const; 
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};