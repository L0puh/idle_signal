#ifndef ENTITY_H
#define ENTITY_H 

#include "core.hpp"
#include <string>

class Model;
struct component_t {
   int collision_floor;
   int collision_main;
   Model* main;
   Model* floor;
};

class Entity {
   public:
      glm::mat4* model;
      
      float rotation_angle = 0.0f;
      glm::vec3 pos, rotation, size;
      component_t component;

   public: 
      Entity(const std::string& filename): pos(glm::vec3(0.0f)), size(glm::vec3(1.0f)) {
         load_entity(filename);
         init_physics();
      }
      ~Entity() {};
   public:
      void update();
      void load_entity(const std::string& filename);
      void draw_entity();
      void init_physics();
};
#endif 
