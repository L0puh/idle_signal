#ifndef MAP_HPP
#define MAP_HPP 

#include <imgui/imgui.h>
#include <vector>

#include "object.hpp"
#include "state.hpp"
#include "camera.hpp"

namespace imgui_color {
   const ImU32 yellow = IM_COL32(255, 255, 0, 255);
   const ImU32 red = IM_COL32(255, 0, 0, 255); 
   const ImU32 white = IM_COL32(255, 255, 255, 255);
   const ImU32 black = IM_COL32(0, 0, 0, 255);
   const ImU32 green = IM_COL32(0, 255, 0, 255);
}


class Map {
   private:
      Shader *shd;
      Texture *tex_wall, *tex_floor;
      
      int state_drawing = wall;
      bool is_drawing=false, show_camera=true;
      
      float scale = 10.0f;
      ImVec2 current_start = ImVec2(0.0f, 0.0f);
     
      std::vector<std::pair<ImVec2, ImVec2>> lines;
      std::vector<std::pair<ImVec2, ImVec2>> floors;
      std::vector<std::pair<glm::vec3, glm::vec3>> walls_obj, floors_obj;

   public:
      Map() {}

   public:
      void set_wall_texure(Texture *tex) { this->tex_wall = tex; }
      void set_floor_texture(Texture *tex) { this->tex_floor = tex; }
      void set_shader(Shader *shd) {this->shd = shd; }
      std::vector<std::pair<glm::vec3, glm::vec3>> get_walls() { return walls_obj; }
   public:
      void draw_objects();
      void add_floor(ImVec2 pos, ImDrawList* draw_list);
      void add_wall(ImVec2 pos, ImDrawList* draw_list);
      void generate_coords(){
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
      }
      void update() {
         if (state.mode & EDIT_MODE){
            editor_popup();
         }
      }
      void editor_popup();
};

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color);

#endif 
