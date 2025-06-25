#ifndef MAP_HPP
#define MAP_HPP 
#include "objects/terrain.hpp"
#include "objects/object.hpp"
#include "utils/resources.hpp"
#include "core/state.hpp"

#include <vector>
#include <imgui.h>

namespace imgui_color {
   const ImU32 yellow = IM_COL32(255, 255, 0, 255);
   const ImU32 red = IM_COL32(255, 0, 0, 255); 
   const ImU32 white = IM_COL32(255, 255, 255, 255);
   const ImU32 black = IM_COL32(0, 0, 0, 255);
   const ImU32 green = IM_COL32(0, 255, 0, 255);
}


class Map {
   protected:
      static Map* instance;
      Map () {}
   public:
      static Map* get_instance() {
         if (instance == NULL) {
            instance = new Map();
         }
         return instance;
      }

   private:
      struct item_t {
         models_type type;
         ImVec2 pos;

      };
      struct entity_t {
         bool is_selected;
         float size = 1.0f;
         std::vector<ImVec2> pos;
      };

   private:
      Shader *shd;
      entity_t *last_entity_to_edit = NULL; 
      
      int state_drawing = item;
      int random_item = NONE;
      models_type item_type_to_draw = NONE;
      bool is_drawing=false, show_camera_pos=true;
      int amount_random_items = 0;

      /* objects in map */
      std::vector<item_t> items;
      std::map<std::string, entity_t> entities;
      
      /* objects in terrain (drawn after closing map) */
      std::vector<object_t> items_obj;
      std::vector<object_t> entities_obj;

      glm::vec2 mouse_pos_world;  //mouse position in the world
      ImVec2 mouse_pos_map;       //mouse position on canvas
   public:
      float terrain_size; 
      float offset = 2.0f;

   public:
      void init(){
         terrain_size = Terrain::get_instance()->width;
         shd = Resources::get_instance()->shaders[MAP_SHADER];
      }
      /* UI ELEMENTS: */
      void editor_popup();
      void show_tabs();
      void edit_terrain();
      void draw_entites_on_map(ImDrawList* draw_list);
      void open_edit_item(entity_t *entity);
      
      /* MAP PROCESS */
      void generate_coords();
      std::string list_entities();
      void draw_objects_on_terrain();
      void process_pickables(Entity*);
      void generate_random_items();
      
      void add_item(models_type type, ImVec2 pos, ImDrawList* draw_list, glm::vec2 projected_pos);
      void add_entity(std::string &entity, glm::vec2 proj_pos);
     
      void update() {
         if (state.mode & EDIT_MODE){
            editor_popup();
         }
      }
   private:

      bool is_item_is_hovered(glm::vec2, glm::vec2);
      models_type popup_items();
};

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color);

#endif 
