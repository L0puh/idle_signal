#include "core.hpp"

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

   return VBO;
}

int Vertex::create_EBO(const void* data, size_t size, GLenum type){
   bind();

   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);

   this->with_EBO = true;

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
void Vertex::add_atrib(uint id, GLint size, GLenum type)
{
   bind();
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   //FIXME: caclulate the proper offset 
   glVertexAttribPointer(id, size, type, GL_FALSE, (size * sizeof(float)), (void*)0);
   glEnableVertexAttribArray(id);
   unbind();
}
