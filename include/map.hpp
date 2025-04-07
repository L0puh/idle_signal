#ifndef MAP_HPP
#define MAP_HPP 

#include <imgui/imgui.h>
#include <vector>
#include "terrain.hpp"
#include "object.hpp"
#include "resources.hpp"
#include "state.hpp"

namespace imgui_color {
   const ImU32 yellow = IM_COL32(255, 255, 0, 255);
   const ImU32 red = IM_COL32(255, 0, 0, 255); 
   const ImU32 white = IM_COL32(255, 255, 255, 255);
   const ImU32 black = IM_COL32(0, 0, 0, 255);
   const ImU32 green = IM_COL32(0, 255, 0, 255);
}

struct item_t {
   models_type type;
   ImVec2 pos;
   ImVec2 map_pos = {-1.0f, -1.0f};

};

class Map {
   private:
      Shader *shd;
     
      int state_drawing = item;
      models_type item_type = NONE;
      bool is_drawing=false, show_camera=true;
      
      std::vector<item_t> items;
      std::vector<object_t> items_obj;
   public:
      float scale = 400.0f;
      float offset = 0.0f; // DELETEME
   public:
      Map() { 
         state.map = this;
         init(); 

      }
      ~Map() {
         delete shd;
      }

   public:
      void set_shader(Shader *shd) {this->shd = shd; }
   public:
      void draw_objects();
      void add_item(models_type type, ImVec2 pos, ImDrawList* draw_list);
      void generate_coords();
      void generate_random_items();
      void update() {
         if (state.mode & EDIT_MODE){
            editor_popup();
         }
      }
      void editor_popup();
   private:
      models_type popup_items();
      void init(){
         scale = state.terrain->width;
         shd = state.resources->shaders[MAP_SHADER];
      }
};

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color);

#endif 
