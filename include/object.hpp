#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "core.hpp"
#include "resources.hpp"

#include <cstdio>

typedef enum {
   item,
   entity

} object_e;


struct object_t {
   std::string name; // for entity
   glm::vec3 max;
   glm::vec3 min;
   
   uint bt_object;
   Model *model;
};

class Line {
   public:
      glm::mat4 model;
      Shader *shd;
      Vertex vert;
      glm::vec3 pos;
      glm::vec3 size;
      glm::vec3 rotation = glm::vec3(1.0f);
      float rotation_angle = 0.0f;
      int count_vertices = 0;
      glm::vec3 color;


   public:
      Line(glm::vec3 from, glm::vec3 to)
      {
         init(from, to);
      }
   void set_data(line_data_t data){
      pos = data.pos;
      size = data.size;
      rotation = data.rotation;
      rotation_angle = data.rotation_angle;
   }
   void init(glm::vec3 from, glm::vec3 to);
   void cleanup() { vert.cleanup(); }
   void draw(GLenum mode = GL_TRIANGLES);
   void set_color(glm::vec3 c) { color = c; }
   void set_size(glm::vec3 s) { size = s; }
   void set_pos(glm::vec3 p) { pos = p; }
   void update() { 
      model = glm::mat4(1.0f); 
      model = glm::translate(model, pos);
      model = glm::rotate(model, rotation_angle, rotation);
      model = glm::scale(model, size);
   }

};

class Text {
   public:
      Shader *shd; 
      Texture *texture;
      Vertex vert;
      glm::vec3 pos, rotation, size;
   private:
      glm::mat4 model = glm::mat4(1.0f);
      bool with_texture = false;
      int count_vertices;
      float rotation_angle = 0.0f;
      glm::vec4 color;

   public:
      Text() {
         shd = state.resources->shaders[TEXT_SHADER];
         texture = state.resources->textures[TEXT_TEXTURE];
         init();
      }
      ~Text(){
         cleanup();
      };


   public:
      void cleanup(){
         vert.cleanup();
      }
      void init();
      void draw(GLenum mode = GL_TRIANGLES);
      void update() { 
         model = glm::mat4(1.0f); 
         model = glm::translate(model, pos);
         model = glm::rotate(model, rotation_angle, rotation);
         model = glm::scale(model, size);
      }
      void set_texture(Texture *texture) { this->texture = texture; }
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
