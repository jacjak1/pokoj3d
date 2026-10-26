#include "model.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(const std::string &path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        path.c_str())) {
    throw std::runtime_error(warn + err);
  }

  std::vector<float> vertices;
  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      // Pozycja
      vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
      vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
      vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

      // Wspolrzedne tekstury (odwrocenie osi Y)
      if (index.texcoord_index >= 0) {
        vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
        vertices.push_back(1.0f -
                           attrib.texcoords[2 * index.texcoord_index + 1]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      // Normalne
      if (index.normal_index >= 0) {
        vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
        vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
        vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
      }
    }
  }

  vertex_count_ = static_cast<GLsizei>(vertices.size() / 8);

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

Model::~Model() {
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void Model::draw() {
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
  glBindVertexArray(0);
}
