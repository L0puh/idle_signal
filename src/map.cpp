#include "map.hpp"
#include "camera.hpp"

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
      draw_grid(draw_list, ImGui::GetCursorScreenPos(), 20.f, imgui_color::white);

      ImGui::InvisibleButton("canvas", ImGui::GetContentRegionAvail());
      ImVec2 pos = ImGui::GetMousePos();
      if (is_drawing){
         if (!ImGui::IsMouseDown(0)){
            is_drawing = false;
            lines.push_back({current_start, pos});
         }
         else 
            draw_list->AddLine(current_start, pos, imgui_color::red, 2.0f);
        }
        if (ImGui::IsItemHovered()) {
            if (!is_drawing && ImGui::IsMouseClicked(0)){
                current_start = pos;
                is_drawing = true;
            }
        }

      for (const auto& l: lines)
         draw_list->AddLine(l.first, l.second, imgui_color::yellow, 2.0f);
      ImGui::End();
   }

}


//                                 UTILS:                                 //

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color){
   int width, height;
   width = state.camera->window_width, height = state.camera->window_height;
   height /= cell_size;
   width /= cell_size;

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

