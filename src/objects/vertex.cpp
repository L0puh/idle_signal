#include "objects/vertex.hpp"
#include "objects/model.hpp"


void Vertex::sub_data(const void* data, size_t size){
   bind_vbo();
   glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
   unbind_vbo();
}
int Vertex::create_VAO(){
   glGenVertexArrays(1, &VAO);
   return VAO;
}

int Vertex::cleanup(){
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   return 0;
}

int Vertex::create_VBO(const void* data, size_t size, GLenum type){
   bind();

   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, size, data, type);
   
   unbind();

   return VBO;
}

int Vertex::create_EBO(const void* data, size_t size, GLenum type){
   bind();

   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);

   this->with_EBO = true;
   
   unbind();

   return EBO;
}

int Vertex::draw_EBO(GLenum mode, size_t size){
   bind();
   glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
   unbind();
   return 0;
}
int Vertex::draw_VBO(GLenum mode, size_t size){
   bind();
   glDrawArrays(mode, 0, size);
   unbind();
   return 0;
}
void Vertex::update_data(const void *data, size_t sz){
   bind();
   glBufferSubData(GL_ARRAY_BUFFER, 0, sz, data);
   unbind();
}

void Vertex::add_atribi(uint id, GLint size, GLenum type, GLsizei stride, void* offset){
   bind();
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   if (stride == 0) stride = size * sizeof(float);
   glVertexAttribIPointer(id, size, type, stride, offset);
   glEnableVertexAttribArray(id);
   unbind();
}
void Vertex::add_atrib(uint id, GLint size, GLenum type, GLsizei stride, void* offset)
{
   bind();
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   if (stride == 0) stride = size * sizeof(float);
   glVertexAttribPointer(id, size, type, GL_FALSE, stride, (void*)offset);
   glEnableVertexAttribArray(id);
   unbind();
}

void Vertex::setup_mesh(Mesh *mesh){
   create_VBO(&mesh->vertices[0], mesh->vertices.size() * sizeof(data_t));
   create_EBO(&mesh->indices[0], mesh->indices.size() * sizeof(uint));
   if (mesh->is_animated){
      add_atrib(0, 3, GL_FLOAT, sizeof(data_t), 0);
      add_atrib(1, 3, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, normal)); 
      add_atrib(2, 2, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, texcoord)); 
      add_atribi(3, 4, GL_INT,   sizeof(data_t), (void*)offsetof(data_t, bone_ids)); 
      add_atrib(4, 4, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, weights)); 
   } else {
      add_atrib(0, 3, GL_FLOAT, sizeof(data_t), 0);
      add_atrib(1, 3, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, normal)); 
      add_atrib(2, 2, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, texcoord)); 
   }
}
