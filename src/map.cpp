#include "map.hpp"
#include "camera.hpp"
#include "glm/geometric.hpp"
#include "object.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include "terrain.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <vector>

void Map::edit_terrain(){
   ImGui::PushItemWidth(100.0f);
   ImGui::InputFloat("max height", &state.terrain->MAX_HEIGHT, 0.1f, 0.2f, "%.3f"); ImGui::SameLine();
   ImGui::InputFloat("min height", &state.terrain->MIN_HEIGHT, 0.1f, 0.2f, "%.3f");
   ImGui::InputInt("max radius",   &state.terrain->MAX_RADIUS, 1, 2); ImGui::SameLine();
   ImGui::InputInt("min radius",   &state.terrain->MIN_RADIUS, 1, 2); 
   ImGui::PopItemWidth();

   if (ImGui::Button("regenerate")){
      state.terrain->generate();
   }

}
std::string Map::list_entities(){
    std::string path = "assets/entities", current = "";

    for (const auto & file: std::filesystem::directory_iterator(path)){
         std::string path = file.path();
         Entity *e = state.resources->entities[path];

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

void Map::show_tabs(){
   if (ImGui::BeginTabBar("sections", 0)) {
      if (ImGui::BeginTabItem("default")){
         if (ImGui::Button("CLEAR")){ 
            items.clear();
         }
         ImGui::SameLine();
         ImGui::Checkbox("show camera", &show_camera); ImGui::SameLine();
         ImGui::DragFloat("offset", &offset, 0.2f, 0.0f, 5.0f, "%.3f"); 

         if (ImGui::Button("add item") && !is_drawing){ 
            state_drawing = item;
            ImGui::OpenPopup("items");
         }
         item_type = popup_items();
         ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("terrain")){
         generate_random_items();
         edit_terrain();
         ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("entity")){
         std::string n = list_entities();
         if (!n.empty()) {
            add_entity(n, projected_pos); 
         }
         ImGui::EndTabItem();
      }
   }
   ImGui::EndTabBar();
   ImGui::SetCursorPos(ImVec2(pos.x + 10.4f, pos.y + 30.0f));
   ImGui::Text("%.2f %.2f\n", projected_pos.x, projected_pos.y);
}

void Map::generate_random_items(){
   
   int size = amount_random_item;
   ImGui::PushItemWidth(100.0f);
   ImGui::RadioButton("TREE", &random_item, models_type::TREE); ImGui::SameLine();
   ImGui::RadioButton("ROCK", &random_item, models_type::ROCK); ImGui::SameLine();
   ImGui::RadioButton("BUSH", &random_item, models_type::BUSH); ImGui::SameLine(); 
   ImGui::InputInt("AMOUNT:", &size, 1, 10); ImGui::SameLine();
   ImGui::PopItemWidth();
   
   std::vector<glm::vec2> coords;
   if (ImGui::Button("generate")){
      if (size < 10000 && size != 0 && random_item != NONE) {
         state.terrain->generate_random_coordinates(size, &coords);

         for (const auto& item: coords){
            items.push_back({(models_type)random_item, {item.x, item.y}});
         }
         log_info("random generation done");
      }
   }
   amount_random_item = size;
}

void Map::open_edit_item(entity_t *entity){

   if(ImGui::BeginPopup("EDIT ITEM"))
   {
      ImGui::DragFloat("SIZE:", &entity->size, 0.5, 0.0f, 10.0f);
      for (int i = 0; i < entity->pos.size(); i++){
         ImGui::PushID(i);
         ImGui::DragFloat2("POS:", &entity->pos[i].x, 0.5, 0.0, state.terrain->width * offset);
         ImGui::PopID();
      }
      ImGui::EndPopup();
   }
}


void Map::editor_popup(){
   ImGui::SetNextWindowSize(ImVec2(state.terrain->height * offset, state.terrain->width * offset), ImGuiCond_Always);
   ImGui::SetNextWindowPos(ImVec2(1, 1), ImGuiCond_Always); 
   ImGui::Begin("map editor", nullptr, 
                  ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse );
   {
      
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
     
      if (ImGui::IsMouseClicked(1)){
         item_type = NONE;
      }
      for (const auto& i: items){
         draw_list->AddCircle(ImVec2(i.pos.x * offset, i.pos.y * offset), 3.2f, imgui_color::yellow, 30, 2.0f);
      }
      for (auto& e: entities){
         for (const auto& n: e.second.pos){
            bool timesout = glfwGetTime() - state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] > state.cooldown;
            bool is_hovered = is_item_is_hovered(glm::vec2(pos.x, pos.y), {n.x*offset, n.y*offset});
            if (is_hovered && state.keys[GLFW_MOUSE_BUTTON_LEFT] && timesout && last_to_edit == NULL){
               state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] = glfwGetTime();
               last_to_edit = &e.second;
               break;
            } 
            draw_list->AddCircle(ImVec2(n.x * offset, n.y * offset), 3.2f, imgui_color::yellow, 30, 2.0f);
         }
      }
      if (state.keys[GLFW_MOUSE_BUTTON_RIGHT]){
         last_to_edit = NULL;
      }
      if (last_to_edit != NULL){
         ImGui::OpenPopup("EDIT ITEM");
         open_edit_item(last_to_edit);
      } 

      draw_grid(draw_list, {0.0f, 0.0f}, 50.f, imgui_color::black);
      show_tabs(); 
      ImGui::InvisibleButton("canvas", ImGui::GetContentRegionAvail());

      pos = ImGui::GetMousePos();
      projected_pos = state.camera->project(pos.x, pos.y, {state.terrain->height * offset, state.terrain->width * offset}) * glm::vec2(scale);
     
      if (show_camera){
         glm::vec3 pp = state.camera->last_pos * offset;
         draw_list->AddCircle(ImVec2(pp.x, pp.z), 5.0f, imgui_color::red, 30, 2.0f);
      }
      switch(state_drawing){
         case object_e::item:
            add_item(item_type, pos, draw_list, projected_pos);
            break;

      }
      ImGui::End();
   }
}


bool Map::is_item_is_hovered(glm::vec2 mouse_pos, glm::vec2 item_pos){
   return glm::distance(mouse_pos, item_pos) <= 4.0f;
}
void Map::add_item(models_type type, ImVec2 pos, ImDrawList* draw_list, glm::vec2 projected_pos){
   if (ImGui::IsMouseClicked(0) && type != NONE){
      items.push_back({type, ImVec2(projected_pos.x, projected_pos.y)});
   }
}

models_type Map::popup_items(){
   models_type object = item_type;
   if (ImGui::BeginPopup("items")) {
        if (ImGui::MenuItem("tree")) {
           object = TREE;
        }
        if (ImGui::MenuItem("rock")) {
           object = ROCK;
        }
        if (ImGui::MenuItem("building")) {
           object = BUILDING;
        }
        if (ImGui::MenuItem("bush")) {
           object = BUSH;
        }
        ImGui::EndPopup();
    }
   return object;
}

void Map::draw_objects(){
   for (const auto& item: items_obj){
      item.model->set_pos(item.max);;
      item.model->set_size(glm::vec3(1.0f));
      item.model->draw();
   }
   for (auto& ent: entities_obj){
      Entity* e = state.resources->entities[ent.name];
      e->set_pos(ent.max);
      e->set_size(glm::vec3(ent.size));
      if (e->is_pickable){
         process_pickables(e);

         float y = state.terrain->get_height_at(e->pos.x, e->pos.z) - 1.0f;
         ent.max = glm::vec3(e->pos.x, y, e->pos.z);
      }
      e->draw_entity();
   }
}


void Map::process_pickables(Entity *ent){
   Camera *cam = state.camera;

   if (!cam->is_picked_object && cam->is_close_to_object(ent->pos, 5.0f)
       && cam->is_pointing_to_object(ent->pos) && !ent->is_picked)
   {
      state.renderer->add_text({"PICK UP (E)", {state.camera->window_width/2.0f,
                  state.camera->window_height/2.0f - 40.0f }, 0.5, color::white});
      if (state.keys[GLFW_KEY_E]){
         ent->is_picked = true;
         cam->is_picked_object = true;
      }
      return;
   }

   if (state.keys[GLFW_KEY_E] && ent->is_picked){
      ent->set_pos(cam->pos + cam->front);
      ent->set_size(glm::vec3(0.4f));
   } else if (!state.keys[GLFW_KEY_E] && ent->is_picked){
      ent->set_pos(cam->pos + cam->front * 1.5f);
      ent->is_picked = false;
      cam->is_picked_object = false;
   }
}



void Map::generate_coords(){
   float y;
   Model* model;
   object_t obj;
   state.physics->clear_objects();
   
   items_obj.clear();
   entities_obj.clear();
   for (int i = 0; i < items.size(); i++){
      glm::vec2 p = glm::vec2(items[i].pos.x, items[i].pos.y);
      y = state.terrain->get_height_at(p.x, p.y);


      size_t sz = state.resources->models[items[i].type].size();
      model = state.resources->models[items[i].type][get_random_int(0, sz-1)];
      model->set_pos(obj.max);
      model->set_size(glm::vec3(1.0f));
      
      obj.max = glm::vec3{p.x, y-1.0f, p.y};
      obj.bt_object = state.physics->add_model(*model);

      obj.model = model;
      items_obj.push_back(obj);

      state.physics->update_position(obj.bt_object, obj.max);
      state.physics->update_size(obj.bt_object, glm::vec3(1.0f));
   }

   for (const auto& ent: entities){
      std::vector<ImVec2> pp = ent.second.pos;
      object_t obj;
      state.resources->entities[ent.first]->init_physics();
      for (const auto& p: pp){
         y = state.terrain->get_height_at(p.x, p.y)-1.0;
         obj.name = ent.first;
         obj.max = {p.x, y, p.y};
         obj.size = ent.second.size;
         entities_obj.push_back(obj);
      }
   }

   log_info("items are loaded");
}

