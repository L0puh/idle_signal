#ifndef LIGHT_H
#define LIGHT_H 

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
      std::vector<glm::vec3> point_light_pos;
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
      void add_point_light(glm::vec3 pos);



};

#endif 
