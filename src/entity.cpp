#include "entity.hpp"
#include "model.hpp"
#include "physics.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void Entity::update() {

   for (int i = 0; i < component.collision.size(); i++){
      state.physics->update_position(component.collision[i], pos);
      state.physics->update_size(component.collision[i], size);
   }

   if (is_floor) {
      component.floor->pos = pos;
      component.floor->size = size;
      component.floor->rotation = rotation;
   }
   
   if (is_wall) {
      component.walls->pos = pos;
      component.walls->size = size;
      component.walls->rotation = rotation;
   }
   
   component.main->pos = pos;
   component.main->size = size;
   component.main->rotation = rotation;
}

void Entity::load_entity(const std::string& filename){
   json doc;
   std::ifstream file(filename, std::ios::in);

   try {
      doc = json::parse(file);
   } catch(const std::exception& e) {
      throw std::runtime_error("failed to parse JSON: " + std::string(e.what()));
   }

   name = doc["name"];
   is_floor    = doc["is_floor"];
   is_pickable = doc["is_pickable"];
   is_door     = doc["is_door"];
   is_wall     = doc["is_wall"]; 

   if (is_floor)
      component.floor = new Model(doc["floor_path"]);
   if (is_door)
      component.door = new Model(doc["door_path"]);
   if (is_wall) {
      component.walls = new Model(doc["wall_path"]);
   }

   component.main = new Model(doc["main_path"]);

}
void Entity::draw_entity(){
   update();

   if (is_floor) 
      component.floor->draw();
   
   if (is_wall){
      component.walls->draw();
      component.walls->draw_debug();
   }
   
   component.main->draw();
   component.main->draw_debug();
}

void Entity::init_physics(){
   Physics* physics = state.physics;
  
   if (is_floor)
      component.collision.push_back(physics->add_model(*component.floor, FLOOR));
   if (is_wall)
      component.collision.push_back(physics->add_model(*component.walls, DEFAULT)); 
  
   // component.collision.push_back(physics->add_model(*component.main, NOTHING));
}




