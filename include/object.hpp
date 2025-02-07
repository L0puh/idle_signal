#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "core.hpp"
#include "vertices.hpp"

typedef enum {
   cube,
   triangle,
   rectangle
} object_e;

class Object {
   private:
      glm::mat4 model = glm::mat4(1.0f);
      Shader shd;
      Vertex vert;

      int count_vertices;
      float rotation_angle;
      glm::vec3 pos, rotation, size;
      glm::vec4 color;

   public:
      Object(){}
      Object(object_e type){
         switch(type){
         case cube:
            {
               shd.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
               vert.create_VBO(vertices::cube, sizeof(vertices::cube));
               vert.add_atrib(0, 3, GL_FLOAT);
               this->count_vertices = LEN(vertices::cube);
            }
            break;

         case triangle: 
            {
               shd.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
               vert.create_VBO(vertices::triangle, sizeof(vertices::triangle));
               vert.add_atrib(0, 3, GL_FLOAT);
               this->count_vertices = LEN(vertices::triangle);
            }
            break;
         case rectangle: 
            {
               shd.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
               vert.create_VBO(vertices::rectangle, sizeof(vertices::rectangle));
               vert.add_atrib(0, 3, GL_FLOAT);
               this->count_vertices = LEN(vertices::rectangle);
            }
           break;
         }
      }
      ~Object(){};

   public:
      void update() { 
         model = glm::mat4(1.0f); 
         model = glm::translate(model, pos);
         model = glm::rotate(model, rotation_angle, rotation);
         model = glm::scale(model, size);
      }
      void draw_object(GLenum mode = GL_TRIANGLES);
      void set_size(glm::vec3 size) { 
         this->size = size; 
      }
      void set_pos(glm::vec3 pos) { 
         this->pos = pos; 
      }
      void set_color(glm::vec4 color) { 
         this->color = color; }
      void set_color(const GLfloat color[4]) { 
            this->color = {color[0], color[1], color[2], color[3]}; }
      void set_rotation(float angle, glm::vec3 pos) {
         this->rotation_angle = angle;
         this->rotation = pos;
      }

};

#endif
