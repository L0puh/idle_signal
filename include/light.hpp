#ifndef LIGHT_H
#define LIGHT_H 

#include "core.hpp"
#include "state.hpp"
#include <vector>

struct light_t{
   glm::vec3 pos;
   glm::vec3 color;
   glm::vec3 direction;
   glm::vec3 view_pos;
   float cut_off;
   float outer_cut_off;
   float dist;

   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

class Light {
   public:
      std::vector<light_t> point_lights;
      light_t flashlight;

   public:
      Light() { 
         init_flashlight();
         state.light = this;
      }
      ~Light() {}
   public:
      void set_all(Shader* shd) {
         set_flashlight(shd);
         set_light(shd);
      }
      void init_flashlight();
      light_t get_flashlight() { return flashlight; }
      void set_light(Shader* shd);
      void set_flashlight(Shader *shd);
      void clear_point_lights() { point_lights.clear(); }
      
      int add_point_light(glm::vec3 pos, const GLfloat* color = color::blue);
      int add_point_light(glm::vec3 pos, glm::vec3 color);
      void update_light_pos(glm::vec3 pos, int id);
      light_t* get_light(int id);

};

#endif 
