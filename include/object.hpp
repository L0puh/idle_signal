#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "core.hpp"
#include "vertices.hpp"

typedef enum {
   wall,
   text,
   line
} object_e;



/* TODO: needs refactoring... */
class Object {
   public:
      Shader *shd; 
      Texture *texture;
      Vertex vert;
   private:
      glm::mat4 model = glm::mat4(1.0f);
      object_e type;
      bool with_texture = false;
      int count_vertices;
      float rotation_angle;
      glm::vec3 pos, rotation, size;
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
            glm::vec3 min = {}, glm::vec3 max = {}): 
         shd(shd), texture(tex)
      {
         if (texture != NULL) with_texture = true;
         switch(type){
         case wall:
            {
               const float vertices[] = {
                  max.x, max.y, max.z, 1.0f, 1.0f,
                  max.x, min.y, max.z, 1.0f, 0.0f,
                  min.x, min.y, max.z, 0.0f, 0.0f,
                  min.x, max.y, max.z, 0.0f, 1.0f
               };
               
               vert.create_VBO(vertices, sizeof(vertices));
               vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
               vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); //pos
               vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float))); //tex
               this->count_vertices = LEN(vertices);
               break;
            }
         case text:
            {
               texture->load_font();
               shd->init_shader(TEXT_SHADER_VERT, TEXT_SHADER_FRAG);
               vert.create_VBO(NULL, sizeof(float) * 4 * 6, GL_DYNAMIC_DRAW);
               vert.add_atrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);
               break;
            }
         default:
            break;
         }
      }

      ~Object(){
         cleanup();
      };

      void cleanup(){
         vert.cleanup();
      }

   public:
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
