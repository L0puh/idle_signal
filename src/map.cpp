#include "map.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "object.hpp"
#include <imgui/imgui.h>

void Map::editor_popup(){
   ImGui::SetNextWindowSize(ImVec2(state.camera->window_width, state.camera->window_height), ImGuiCond_Always);
   ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); 
   ImGui::Begin("map editor", nullptr, 
                  ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
   {
      
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      if (ImGui::Button("CLEAR")){ 
         floors.clear();
         lines.clear();
      }
      ImGui::SameLine();
      ImGui::Checkbox("Show camera", &show_camera); ImGui::SameLine();
      ImGui::Checkbox("Show roof", &show_roof); ImGui::SameLine();
      ImGui::DragFloat("Scale", &scale, 1.0f, 0.0f, 100.0f, "%.3f"); 
      ImGui::Text("Choose object:"); ImGui::SameLine();
      ImGui::RadioButton("Wall", &state_drawing, object_e::wall); ImGui::SameLine();
      ImGui::RadioButton("Floor", &state_drawing, object_e::tiles); ImGui::SameLine();
      ImGui::RadioButton("Door", &state_drawing, object_e::door); ImGui::SameLine();
      ImGui::RadioButton("Roof", &state_drawing, object_e::roof);


      draw_grid(draw_list, ImGui::GetCursorScreenPos(), 20.f, imgui_color::white);
      ImGui::InvisibleButton("canvas", ImGui::GetContentRegionAvail());
      ImVec2 pos = ImGui::GetMousePos();
     
      if (show_camera){
         //FIXME:
         draw_list->AddCircle(ImVec2(pos.x, pos.y), 10.0f, imgui_color::red, 30, 2.0f);
      }
      switch(state_drawing){
         case object_e::wall: 
            add_wall(pos, draw_list);
            break;

         case object_e::tiles:
            add_floor(pos, draw_list);
            break;
         case object_e::roof:
            add_roof(pos, draw_list);
            break;

      }
      
      for (const auto& f: floors){
         draw_list->AddRect(f.first, f.second, imgui_color::green, 2.0f);
      }
      for (const auto& l: lines){
         draw_list->AddLine(l.first, l.second, imgui_color::yellow, 2.0f);
      }
      if (show_roof)
         for (const auto& r: roof){
            draw_list->AddRect(r.first, r.second, imgui_color::red, 1.0f);
         }
      ImGui::End();
   }
}

void Map::add_roof(ImVec2 pos, ImDrawList* draw_list){
  if (is_drawing) {
      if (!ImGui::IsMouseDown(0)) {
          is_drawing = false;
          roof.push_back({ current_start, pos });
      } else if (ImGui::IsMouseDown(1)) {
          is_drawing = false;
      } else {
          draw_list->AddRect(current_start, pos, imgui_color::red);
      }
  }
  if (ImGui::IsItemHovered()) {
      if (!is_drawing && ImGui::IsMouseClicked(0)){
          current_start = pos;
          is_drawing = true;
      }
  }
}

void Map::add_floor(ImVec2 pos, ImDrawList* draw_list){
  if (is_drawing) {
      if (!ImGui::IsMouseDown(0)) {
          is_drawing = false;
          floors.push_back({ current_start, pos });
      } else if (ImGui::IsMouseDown(1)) {
          is_drawing = false;
      } else {
          draw_list->AddRect(current_start, pos, imgui_color::red);
      }
  }
  if (ImGui::IsItemHovered()) {
      if (!is_drawing && ImGui::IsMouseClicked(0)){
          current_start = pos;
          is_drawing = true;
      }
  }
}

void Map::add_wall(ImVec2 pos, ImDrawList* draw_list){
   if (is_drawing){
      if (!ImGui::IsMouseDown(0)){
         is_drawing = false;
         lines.push_back({current_start, pos});
      } else if (ImGui::IsMouseDown(1)){
         is_drawing = false;
      } else 
         draw_list->AddLine(current_start, pos, imgui_color::red, 2.0f);
     }
     if (ImGui::IsItemHovered()) {
         if (!is_drawing && ImGui::IsMouseClicked(0)){
             current_start = pos;
             is_drawing = true;
         }
     }

}

void Map::draw_objects(){
   for (const auto& wall: walls_obj){
      glm::vec3 min, max;
      min = glm::vec3(wall.first.x, state.ground_level, wall.first.z);
      max = wall.second;

      Object w(object_e::wall, tex_wall, shd, 
            min, max);
      
      w.set_pos(glm::vec3(0.0f));
      w.set_size(glm::vec3(1.0f));
      w.draw();

   }
   for (const auto& roof: roof_obj){
      Object w(object_e::roof, tex_roof, shd, 
            roof.first, roof.second);

      w.set_pos(glm::vec3(0.0f));
      w.set_size(glm::vec3(1.0f));
      w.draw();

   }
   for (const auto& floor: floors_obj){
      Object w(object_e::tiles, tex_floor, shd, 
            floor.first, floor.second);

      w.set_pos(glm::vec3(0.0f));
      w.set_size(glm::vec3(1.0f));
      w.draw();

   }
}


void Map::generate_coords(){
   walls_obj.clear();
   for (int i = 0; i < lines.size(); i++){
      glm::vec2 p = state.camera->project(lines[i].first.x, lines[i].first.y) * scale; 
      glm::vec2 p2 = state.camera->project(lines[i].second.x, lines[i].second.y) * scale;
      walls_obj.push_back({glm::vec3(p.x, 1.0f, p.y), glm::vec3(p2.x, 1.0f, p2.y)});
   }
   floors_obj.clear(); 
   for (int i = 0; i < floors.size(); i++){
      glm::vec2 p = state.camera->project(floors[i].first.x, floors[i].first.y) * scale; 
      glm::vec2 p2 = state.camera->project(floors[i].second.x, floors[i].second.y) * scale;
      floors_obj.push_back({glm::vec3(p.x, state.ground_level, p.y), glm::vec3(p2.x, state.ground_level, p2.y)});
   }

   for (int i = 0; i < roof.size(); i++){
      glm::vec2 p = state.camera->project(roof[i].first.x, roof[i].first.y) * scale; 
      glm::vec2 p2 = state.camera->project(roof[i].second.x, roof[i].second.y) * scale;
      roof_obj.push_back({glm::vec3(p.x, state.ground_level + roof_level, p.y), glm::vec3(p2.x, state.ground_level + roof_level, p2.y)});
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

