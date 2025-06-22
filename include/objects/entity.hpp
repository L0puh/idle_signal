#ifndef ENTITY_H
#define ENTITY_H 

#include <string>
#include <vector>
#include <glm/glm.hpp>

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
      int light_id = 0;
      std::string name;
      glm::mat4* model;

      bool is_animated = false;
      bool is_picked = false;
      bool is_floor, is_pickable, is_door, is_wall;
      bool is_main_collide = false;
      bool has_light = false;
      glm::vec3 light_color;


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
      void set_pos(glm::vec3 p) { pos = p; }
      void set_size(glm::vec3 s) { size = s; }

      void update();
      void load_entity(const std::string& filename);
      void draw_entity();
      void init_light();
      void init_physics();
};
#endif 
