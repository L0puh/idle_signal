#ifndef ENTITY_H
#define ENTITY_H 

#include "core.hpp"
#include <string>
#include <vector>

class Model;
struct component_t {

   std::vector<int> collision;

   Model*  main;
   Model*  floor;
   Model*  door;
   Model*  walls;
};

class Entity {
   public:
      std::string name;
      glm::mat4* model;

      bool is_floor, is_pickable, is_door, is_wall;

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
