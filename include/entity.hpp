#ifndef ENTITY_H
#define ENTITY_H 

#include "core.hpp"
#include <string>

class Model;
struct component_t {
   Model* main;
   Model* floor;
};

class Entity {
   public:
      glm::mat4* model;
      component_t component;

   public: 
      Entity(const std::string& filename) {
         load_entity(filename);
      }
      ~Entity() {};
   public:
      void load_entity(const std::string& filename);
      void draw_entity();
      void init_physics();
};
#endif 
