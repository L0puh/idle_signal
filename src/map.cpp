#include "map.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "physics.hpp"
#include "terrain.hpp"

#include <imgui/imgui.h>
#include <vector>

void Map::edit_terrain(){
   ImGui::PushItemWidth(100.0f);
   ImGui::InputFloat("max height", &state.terrain->MAX_HEIGHT, 0.1f, 0.2f, "%.3f"); ImGui::SameLine();
   ImGui::InputFloat("min height", &state.terrain->MIN_HEIGHT, 0.1f, 0.2f, "%.3f");
   ImGui::InputInt("max radius", &state.terrain->MAX_RADIUS, 1, 2); ImGui::SameLine();
   ImGui::InputInt("min radius", &state.terrain->MIN_RADIUS, 1, 2); 
   ImGui::PopItemWidth();

   if (ImGui::Button("regenerate")){
      state.terrain->generate();
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
   }
   ImGui::EndTabBar();
   ImGui::SetCursorPos(ImVec2(pos.x + 10.4f, pos.y + 30.0f));
   ImGui::Text("%.2f %.2f\n", projected_pos.x, projected_pos.y);
}

void Map::generate_random_items(){
   
   // TODO randomize the models themselves (more types of trees and stuff)
   int size = amount_random_item;
   ImGui::PushItemWidth(100.0f);
   ImGui::RadioButton("TREE", &random_item, models_type::TREE); ImGui::SameLine();
   ImGui::RadioButton("ROCK", &random_item, models_type::ROCK); ImGui::SameLine();
   ImGui::RadioButton("TRUNK", &random_item, models_type::TREE_TRUNK); ImGui::SameLine();
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
        if (ImGui::MenuItem("trunk")) {
           object = TREE_TRUNK;
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
}


void Map::generate_coords(){
   float y;
   Model* model;
   object_t obj;
   state.physics->clear_objects();
   // TODO: if (has_changed)
   
   items_obj.clear();
   for (int i = 0; i < items.size(); i++){
      glm::vec2 p = glm::vec2(items[i].pos.x, items[i].pos.y);
      y = state.terrain->get_height_at(p.x, p.y);
      model = state.resources->models[items[i].type];
      model->set_pos(obj.max);
      model->set_size(glm::vec3(1.0f));
      
      obj.max = glm::vec3{p.x, y-1.0f, p.y};
      obj.bt_object = state.physics->add_model(*model);

      obj.model = model;
      items_obj.push_back(obj);

      state.physics->update_position(obj.bt_object, obj.max);
      state.physics->update_size(obj.bt_object, glm::vec3(1.0f));
   }
   log_info("items are loaded");
}

//                                 UTILS:                                 //

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color){
   int width, height;
   width = state.camera->window_width, height = state.camera->window_height;
   height /= cell_size;
   width  /= cell_size;

   for (int i = 0; i <= height; i++){
      draw_list->AddLine(
            ImVec2(origin.x, origin.y + i * cell_size),
            ImVec2(origin.x + width * cell_size, origin.y + i * cell_size),
            color, 0.5f
        );
   }

   for (int i = 0; i <= width; i++){
      draw_list->AddLine(
            ImVec2(origin.x + i * cell_size, origin.y),
            ImVec2(origin.x + i * cell_size, origin.y + height * cell_size),
            color, 0.5f
        );
   }
}

