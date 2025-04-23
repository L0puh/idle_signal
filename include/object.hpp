#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "core.hpp"

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

typedef enum {
   text,
   line,
   item,
   normal,
  
   // LEGACY:
   tiles,
   roof,
   wall,

} object_e;


struct object_t {
   std::string name; // for entity
   glm::vec3 max;
   glm::vec3 min;
   
   uint bt_object;
   Model *model;
};

/* TODO: needs refactoring... */
class Object {
   public:
      Shader *shd; 
      Texture *texture;
      Vertex vert;
      glm::vec3 pos, rotation, size;
   private:
      glm::mat4 model = glm::mat4(1.0f);
      object_e type;
      bool with_texture = false;
      int count_vertices;
      float rotation_angle = 0.0f;
      glm::vec4 color;

   public:
      Object(){}

      //for a line
      Object(object_e type, glm::vec3 from, glm::vec3 to, Shader *shd):
                                                shd(shd), type(type)
      {
         const float vertices[] = {
            from.x, from.y, from.z,
            to.x, to.y, to.z
         };
         vert.create_VBO(vertices, sizeof(vertices));
         vert.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float), 0);
         count_vertices = LEN(vertices);
      }


      Object(object_e type, Texture *tex, Shader *shd, 
            glm::vec3 min = {}, glm::vec3 max = {}, bool is_blank = false): 
         shd(shd), texture(tex), rotation_angle(0.0f), rotation(glm::vec3(1.0f)){
         generate_object(type, max, min);
      }

      ~Object(){
         cleanup();
      };

      void cleanup(){
         vert.cleanup();
      }

   public:
      void generate_object(object_e type, glm::vec3 max, glm::vec3 min);
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
