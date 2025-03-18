#ifndef MAP_HPP
#define MAP_HPP 

#include <imgui/imgui.h>
#include <vector>

#include "state.hpp"

namespace imgui_color {
   const ImU32 yellow = IM_COL32(255, 255, 0, 255);
   const ImU32 red = IM_COL32(255, 0, 0, 255); 
   const ImU32 white = IM_COL32(255, 255, 255, 255);
   const ImU32 black = IM_COL32(0, 0, 0, 255);
}

class Map {
   private:
      std::vector<std::pair<ImVec2, ImVec2>> lines;
      ImVec2 current_start = ImVec2(0.0f, 0.0f);
      bool is_drawing=false;
      std::vector<glm::vec3> walls;
   public:
      Map() {}
   public:
      void update() {
         if (state.mode & EDIT_MODE){
            editor_popup();
         }
      }
      void editor_popup();
};

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color);

#endif 
