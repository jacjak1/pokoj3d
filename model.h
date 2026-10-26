#pragma once

#include <string>
#include <vector>
#include <gl/glew.h>

class Model {
public:
    Model(const std::string& path);
    ~Model();
    void draw();

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLsizei vertex_count_ = 0;
};
