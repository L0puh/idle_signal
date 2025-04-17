#include "entity.hpp"
#include "model.hpp"
#include "physics.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

   if (component.floor != NULL) {
      component.floor->draw();
   }
   component.main->draw();
}

void Entity::init_physics(){
   Physics* physics = state.physics;
  
   if (component.floor != NULL)
      physics->add_model(*component.floor, FLOOR);
  
   //FIXME: parse whether it's interactable
   physics->add_model(*component.main, STATIC);
}
