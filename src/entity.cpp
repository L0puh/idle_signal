#include "entity.hpp"
#include "model.hpp"
#include "physics.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void Entity::update() {

   //udpate collision object

   state.physics->update_position(component.collision_main, pos);
   state.physics->update_size(component.collision_main, size);
   state.physics->update_position(component.collision_floor, pos);
   state.physics->update_size(component.collision_floor, size);

   if (component.floor) {
      component.floor->pos = pos;
      component.floor->size = size;
      component.floor->rotation = rotation;
   }

   component.main->pos = pos;
   component.main->size = size;
   component.main->rotation = rotation;
}

void Entity::load_entity(const std::string& filename){
   std::ifstream file(filename, std::ios::in);

   json doc;
   try {
      doc = json::parse(file);
   } catch(const std::exception& e) {
      throw std::runtime_error("failed to parse JSON: " + std::string(e.what()));
   }

   std::string name{doc["name"]};
   int has_floor = doc["has_floor"];
   if (has_floor)
      component.floor = new Model(doc["floor_path"]);

   component.main = new Model(doc["main_path"]);

}
void Entity::draw_entity(){
   update();

   // if (component.floor != NULL) {
   //    component.floor->draw();
   // }
   component.main->draw();
}

void Entity::init_physics(){
   Physics* physics = state.physics;
  
   if (component.floor != NULL)
      component.collision_floor = physics->add_model(*component.floor, FLOOR);
  
   component.collision_main = physics->add_model(*component.main, STATIC);
}
