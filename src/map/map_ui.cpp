#include "core/camera.hpp"
#include "map/map.hpp"

void Map::draw_entites_on_map(ImDrawList* draw_list) {
   for (auto& e: entities){
      for (const auto& n: e.second.pos){
         bool timesout = glfwGetTime() - state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] > state.cooldown;
         bool is_hovered = is_item_is_hovered(glm::vec2(mouse_pos_map.x, mouse_pos_map.y), {n.x*offset, n.y*offset});
         if (is_hovered && state.keys[GLFW_MOUSE_BUTTON_LEFT] && timesout && last_entity_to_edit == NULL){
            state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] = glfwGetTime();
            last_entity_to_edit = &e.second;
            break;
         } 
         draw_list->AddCircle(ImVec2(n.x * offset, n.y * offset), 3.2f, imgui_color::yellow, 30, 2.0f);
      }
   }
   
   for (const auto& i: items){
      draw_list->AddCircle(ImVec2(i.pos.x * offset, i.pos.y * offset), 3.2f, imgui_color::yellow, 30, 2.0f);
   }
}

void Map::editor_popup(){
   /* entry point of map */
   ImGui::SetNextWindowSize(ImVec2(Terrain::get_instance()->height * offset, Terrain::get_instance()->width * offset), ImGuiCond_Always);
   ImGui::SetNextWindowPos(ImVec2(1, 1), ImGuiCond_Always); 
   ImGui::Begin("map editor", nullptr, 
                  ImGuiWindowFlags_NoTitleBar |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse );
   {
      
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      draw_entites_on_map(draw_list);
      if (ImGui::IsMouseClicked(1)){
         item_type_to_draw = NONE;
         last_entity_to_edit = NULL;
      }
     
      if (last_entity_to_edit != NULL){
         ImGui::OpenPopup("EDIT ITEM");
         open_edit_item(last_entity_to_edit);
      } 

      draw_grid(draw_list, {0.0f, 0.0f}, 50.f, imgui_color::black);
      show_tabs(); 
      
      ImGui::InvisibleButton("canvas", ImGui::GetContentRegionAvail());
      mouse_pos_map = ImGui::GetMousePos();
      mouse_pos_world = Camera::get_instance()->project(mouse_pos_map.x, mouse_pos_map.y,
                     {Terrain::get_instance()->height * offset, Terrain::get_instance()->width * offset}) 
                           * glm::vec2(terrain_size);
     
      if (show_camera_pos){
         glm::vec3 pp = Camera::get_instance()->last_pos * offset;
         draw_list->AddCircle(ImVec2(pp.x, pp.z), 5.0f, imgui_color::red, 30, 2.0f);
      }
      if (state_drawing == object_e::item )
         add_item(item_type_to_draw, mouse_pos_map, draw_list, mouse_pos_world);

      ImGui::End();
   }
}
models_type Map::popup_items(){
   /* draw static items to add with editor (not entities) */ 
   /* TODO: clear it out and merge with Entity class      */
   models_type object = item_type_to_draw;
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



void Map::open_edit_item(entity_t *entity){

   if(ImGui::BeginPopup("EDIT ITEM"))
   {
      ImGui::DragFloat("SIZE:", &entity->size, 0.5, 0.0f, 10.0f);
      for (int i = 0; i < entity->pos.size(); i++){
         ImGui::PushID(i);
         ImGui::DragFloat2("POS:", &entity->pos[i].x, 0.5, 0.0, Terrain::get_instance()->width * offset);
         ImGui::PopID();
      }
      ImGui::EndPopup();
   }
}


void Map::show_tabs(){
   if (ImGui::BeginTabBar("sections", 0)) {
      if (ImGui::BeginTabItem("default")){
         if (ImGui::Button("CLEAR")){ 
            items.clear();
         }
         ImGui::SameLine();
         ImGui::Checkbox("show camera", &show_camera_pos); ImGui::SameLine();
         ImGui::DragFloat("offset", &offset, 0.2f, 0.0f, 5.0f, "%.3f"); 

         if (ImGui::Button("add item") && !is_drawing){ 
            state_drawing = item;
            ImGui::OpenPopup("items");
         }
         item_type_to_draw = popup_items();
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
            add_entity(n, mouse_pos_world); 
         }
         ImGui::EndTabItem();
      }
   }
   ImGui::EndTabBar();
   ImGui::SetCursorPos(ImVec2(mouse_pos_map.x + 10.4f, mouse_pos_map.y + 30.0f));
   ImGui::Text("%.2f %.2f\n", mouse_pos_world.x, mouse_pos_world.y);
}

void Map::edit_terrain(){
   ImGui::PushItemWidth(100.0f);
   ImGui::InputFloat("max height", &Terrain::get_instance()->MAX_HEIGHT, 0.1f, 0.2f, "%.3f"); ImGui::SameLine();
   ImGui::InputFloat("min height", &Terrain::get_instance()->MIN_HEIGHT, 0.1f, 0.2f, "%.3f");
   ImGui::InputInt("max radius",   &Terrain::get_instance()->MAX_RADIUS, 1, 2); ImGui::SameLine();
   ImGui::InputInt("min radius",   &Terrain::get_instance()->MIN_RADIUS, 1, 2); 
   ImGui::PopItemWidth();

   if (ImGui::Button("regenerate")){
      Terrain::get_instance()->generate();
   }

}
