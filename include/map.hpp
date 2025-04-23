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

};
struct entity_t {
   bool is_selected;
   std::vector<ImVec2> pos;

};

class Map {
   private:
      Shader *shd;
     
      int state_drawing = item;
      int random_item = NONE;
      models_type item_type = NONE;
      bool is_drawing=false, show_camera=true;
      int amount_random_item = 0;

      
      std::vector<item_t> items;
      std::map<std::string, entity_t> entities;
      std::vector<object_t> items_obj;
      std::vector<object_t> entities_obj;

      glm::vec2 projected_pos; //mouse position in the world
      ImVec2 pos; //mouse position on canvas
   public:
      float scale; // the terrian size!
      float offset = 2.0f;
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
      void show_tabs();
      void edit_terrain();
      void draw_objects();
      void generate_coords();
      std::string list_entities();
      
      void add_item(models_type type, ImVec2 pos, ImDrawList* draw_list, glm::vec2 projected_pos);
      void add_entity(std::string &entity, glm::vec2 proj_pos);
     
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
