#include "texture.h"
#include <gl/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned loadTexture(const char* path)
{
    int w, h, n; unsigned char* data = stbi_load(path, &w, &h, &n, 0);
    unsigned id; glGenTextures(1, &id); glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, n == 4 ? GL_RGBA : GL_RGB, w, h, 0,
        n == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data); return id;
}
