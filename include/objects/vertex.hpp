#ifndef VERTEX_H
#define VERTEX_H 

#include "core/core.hpp"

class Vertex { 
   private:
      uint VAO, VBO, EBO;
   public:
      Vertex()  { 
         VAO = create_VAO(); 
      }
      ~Vertex() { 
         //FIXME cleanup(); 
       }
   public:
      bool with_EBO = false;
   public:
      int create_VAO();
      int create_EBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);
      int create_VBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);

      void bind()       { glBindVertexArray(VAO);}
      void bind_vbo()   { glBindBuffer(GL_ARRAY_BUFFER, VBO);}
      void unbind()     { glBindVertexArray(0);  }
      void unbind_vbo()  { glBindBuffer(GL_ARRAY_BUFFER, 0);  }

      void setup_mesh(Mesh *mesh);
      
      void sub_data(const void* data, size_t size);
      void add_atrib(uint id, GLint size, GLenum type, GLsizei stride, void* offset=0);
      void add_atribi(uint id, GLint size, GLenum type, GLsizei stride, void* offset);
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      void update_data(const void *data, size_t);
      int cleanup();
};
#endif /* ifndef VERTEX_H */
