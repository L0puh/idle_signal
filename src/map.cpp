#include "map.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "physics.hpp"
#include "terrain.hpp"

#include <imgui/imgui.h>

void Map::generate_random_items(){
   std::vector<glm::vec2> trees, rocks, bushes, trunks, wheat;
   state.terrain->generate_random_coordinates(30, &trees);
   state.terrain->generate_random_coordinates(10, &rocks);
   state.terrain->generate_random_coordinates(10, &bushes);
   state.terrain->generate_random_coordinates(20, &trunks);
   state.terrain->generate_random_coordinates(10, &wheat);
   for (auto& item: trunks){
      items.push_back({TREE_TRUNK, {item.x, item.y}});
   }
   for (auto& item: trees){
      items.push_back({TREE, {item.x, item.y}});
   }
   for (auto& item: rocks){
      items.push_back({ROCK, {item.x, item.y}});
   }
   for (auto& item: bushes){
      items.push_back({BUSH, {item.x, item.y}});
   }
   for (auto& item: wheat){
      items.push_back({WHEAT, {item.x, item.y}});
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
     
      ImGui::SameLine();
      if (ImGui::Button("generate random items")){
         generate_random_items();
      }
      if (ImGui::IsMouseClicked(1)){
         item_type = NONE;
      }

      ImGui::InvisibleButton("canvas", ImGui::GetContentRegionAvail());
      ImVec2 pos = ImGui::GetMousePos();
      glm::vec2 projected_pos = state.camera->project(pos.x, pos.y, {state.terrain->height * offset, state.terrain->width * offset}) * glm::vec2(scale);
      ImGui::SetCursorPos(ImVec2(pos.x + 10.4f, pos.y + 30.0f));
      ImGui::Text("%.2f %.2f\n", projected_pos.x, projected_pos.y);
     
      if (show_camera){
         glm::vec3 pp = state.camera->last_pos * offset;
         draw_list->AddCircle(ImVec2(pp.x, pp.z), 5.0f, imgui_color::red, 30, 2.0f);
      }
      switch(state_drawing){
         case object_e::item:
            add_item(item_type, pos, draw_list, projected_pos);
            break;

      }
       for (const auto& i: items){
          draw_list->AddCircle(ImVec2(i.pos.x * offset, i.pos.y * offset), 3.2f, imgui_color::yellow, 30, 2.0f);
       }
      item_type = popup_items();
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
        if (ImGui::MenuItem("wheat")) {
           object = WHEAT;
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

      float y = state.terrain->get_height_at(item.max.x, item.max.y);
      glm::vec3 pos = glm::vec3{item.max.x, y-1.0f, item.max.y};
      item.model->set_pos(pos);
      item.model->set_size(glm::vec3(1.0f));
      item.model->draw();
      state.physics->update_size(item.bt_object, item.model->size);
      state.physics->update_position(item.bt_object, pos);

   }
}


void Map::generate_coords(){
   state.physics->clear_objects();
   items_obj.clear();
   for (int i = 0; i < items.size(); i++){

      glm::vec2 p = glm::vec2(items[i].pos.x, items[i].pos.y);
      Model* model;
      model = state.resources->models[items[i].type];
      uint bt_object = state.physics->add_model(*model);
      object_t obj;
      obj.max = glm::vec3(p, 0.0f);
      obj.bt_object = bt_object;
      obj.model = model;
      items_obj.push_back(obj);

   }
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

