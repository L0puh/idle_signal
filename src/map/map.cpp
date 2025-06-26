#include "core/core.hpp"
#include "map/map.hpp"
#include "core/window.hpp"
#include "utils/renderer.hpp"
#include "core/camera.hpp"
#include "objects/object.hpp"
#include "physics/physics.hpp"
#include "objects/terrain.hpp"

#include <vector>


std::string Map::list_entities(){
    std::string path = "assets/entities", current = "";

    for (const auto & file: std::filesystem::directory_iterator(path)){
         std::string path = file.path();
         Entity *e = Resources::get_instance()->entities[path];

         if (ImGui::Selectable(e->name.c_str(), entities[path].is_selected)){
            entities[path].is_selected = true;
            return path;
         }
         if (entities[path].is_selected)
            current = path;
         
    }
   return current;
}


void Map::add_entity(std::string& entity, glm::vec2 proj_pos){
   if (ImGui::IsMouseClicked(0)){
      entities[entity].pos.push_back(ImVec2(proj_pos.x, proj_pos.y));
      entities[entity].is_selected = false;
   }
}


void Map::generate_random_items(){
   
   int size = amount_random_items;
   ImGui::PushItemWidth(100.0f);
   ImGui::RadioButton("TREE", &random_item, models_type::TREE); ImGui::SameLine();
   ImGui::RadioButton("ROCK", &random_item, models_type::ROCK); ImGui::SameLine();
   ImGui::RadioButton("BUSH", &random_item, models_type::BUSH); ImGui::SameLine(); 
   ImGui::InputInt("AMOUNT:", &size, 1, 10); ImGui::SameLine();
   ImGui::PopItemWidth();
   
   std::vector<glm::vec2> coords;
   if (ImGui::Button("generate")){
      if (size < 10000 && size != 0 && random_item != NONE) {
         Terrain::get_instance()->generate_random_coordinates(size, &coords);

         for (const auto& item: coords){
            items.push_back({(models_type)random_item, {item.x, item.y}});
         }

         Log::get_logger()->info("random generation is done");
      }
   }
   amount_random_items = size;
}


bool Map::is_item_is_hovered(glm::vec2 mouse_pos, glm::vec2 item_pos){
   return glm::distance(mouse_pos, item_pos) <= 4.0f;
}
void Map::add_item(models_type type, ImVec2 pos, ImDrawList* draw_list, glm::vec2 world_pos){
   if (ImGui::IsMouseClicked(0) && type != NONE){
      items.push_back({type, ImVec2(world_pos.x, world_pos.y)});
   }
}

void Map::process_pickables(Entity *ent){
   Camera *cam = Camera::get_instance();

   if (!cam->is_picked_object && cam->is_close_to_object(ent->pos, 5.0f * ent->size.x)
       && cam->is_pointing_to_object(ent->pos) && !ent->is_picked)
   {
      Renderer::get_instance()->add_text({"PICK UP (E)", {Window::get_width()/2.0f,
                  Window::get_height()/2.0f - 40.0f }, 0.5, color::white});
      if (state.keys[GLFW_KEY_E]){
         ent->is_picked = true;
         cam->is_picked_object = true;
      }
      return;
   }

   if (state.keys[GLFW_KEY_E] && ent->is_picked){
      ent->set_pos(cam->pos + cam->front );
      ent->set_size(glm::vec3(0.4f));
   } else if (!state.keys[GLFW_KEY_E] && ent->is_picked){
      ent->set_pos(cam->pos + cam->front * (ent->size.x + 3.0f));
      ent->is_picked = false;
      cam->is_picked_object = false;
   }
}



void Map::generate_coords(){
   float y;
   Model* model;
   object_t obj;
   Physics::get_instance()->clear_objects();
   
   items_obj.clear();
   entities_obj.clear();
   for (int i = 0; i < items.size(); i++){
      glm::vec2 p = glm::vec2(items[i].pos.x, items[i].pos.y);
      y = Terrain::get_instance()->get_height_at(p.x, p.y);


      size_t sz = Resources::get_instance()->models[items[i].type].size();
      model = Resources::get_instance()->models[items[i].type][get_random_int(0, sz-1)];
      model->set_pos(obj.max);
      model->set_size(glm::vec3(1.0f));
      
      obj.max = glm::vec3{p.x, y-1.0f, p.y};
      obj.bt_object = Physics::get_instance()->add_model(*model);

      obj.model = model;
      items_obj.push_back(obj);

      Physics::get_instance()->update_position(obj.bt_object, obj.max);
      Physics::get_instance()->update_size(obj.bt_object, glm::vec3(1.0f));
   }

   for (const auto& ent: entities){
      std::vector<ImVec2> pp = ent.second.pos;
      object_t obj;
      Resources::get_instance()->entities[ent.first]->init_physics();
      for (const auto& p: pp){
         y = Terrain::get_instance()->get_height_at(p.x, p.y)-1.0;
         obj.name = ent.first;
         obj.max = {p.x, y, p.y};
         obj.size = ent.second.size;
         entities_obj.push_back(obj);
      }
   }
   
   Log::get_logger()->info("items are loaded");
}

void Map::draw_objects_on_terrain(){

   for (const auto& item: items_obj){
      item.model->set_pos(item.max);;
      item.model->set_size(glm::vec3(1.0f));
      item.model->draw();
   }
   for (auto& ent: entities_obj){
      Entity* e = Resources::get_instance()->entities[ent.name];
      e->set_pos(ent.max);
      e->set_size(glm::vec3(ent.size));
      if (e->is_pickable){
         process_pickables(e);
         float y = Terrain::get_instance()->get_height_at(e->pos.x, e->pos.z) - 1.0f;
         ent.max = glm::vec3(e->pos.x, y, e->pos.z);
      }
      e->draw_entity();
   }
}

