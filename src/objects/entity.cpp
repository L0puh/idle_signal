#include "objects/entity.hpp"
#include "objects/model.hpp"
#include "physics/physics.hpp"
#include "shaders/light.hpp"


#include <glm/glm.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void Entity::update() {
   for (int i = 0; i < component.collision.size(); i++){

      Physics::get_instance()->update_position(component.collision[i], pos);
      Physics::get_instance()->update_size(component.collision[i], size);
   }

   if (is_floor) {
      component.floor->set_pos(pos);
      component.floor->set_size(size);
   }
   
   if (is_wall) {
      component.walls->set_pos(pos);
      component.walls->set_size(size);
   }

   if (is_door){
      component.door->set_pos(pos);
      component.door->set_size(size);
   }
  
   component.main->set_pos(pos);
   component.main->set_size(size);

   if (has_light) 
      Light::get_instance()->update_light_pos(pos, light_id);
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
   has_light   = doc["has_light"];
   if (doc.contains("is_animated"))
      is_animated = doc["is_animated"];

   is_main_collide = doc["main_collide"];

   if (is_floor)
      component.floor = new Model(doc["floor_path"]);
   if (is_door)
      component.door = new Model(doc["door_path"]);
   if (is_wall)
      component.walls = new Model(doc["wall_path"]);

   if (has_light){
      std::vector<float> p = doc["light_color"];
      light_color = glm::vec3(p[0], p[1], p[2]);
   }
   
   component.main = new Model(doc["main_path"], is_animated);

   if (has_light) init_light();
}

void Entity::draw_entity(){
   update();

   // if (is_floor) 
   //    component.floor->draw();
   
   // if (is_wall){
   //    component.walls->draw();
   // }
   if (is_door)
      component.door->draw_debug();

   component.main->draw();
}

void Entity::init_light(){
   light_id = Light::get_instance()->add_point_light(pos+1.0f, light_color);
}

void Entity::init_physics(){
   Physics* physics = Physics::get_instance();
  
   if (is_floor)
      component.collision.push_back(physics->add_model(*component.floor, FLOOR));
   if (is_wall)
      component.collision.push_back(physics->add_model(*component.walls, DEFAULT)); 
   if (is_door)
      component.collision.push_back(physics->add_model(*component.door, DOOR)); 
 
   if (is_main_collide)
       component.collision.push_back(physics->add_model(*component.main, DEFAULT));
}




