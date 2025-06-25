#include "core/core.hpp"
#include "core/camera.hpp"
#include "core/window.hpp"
#include "shaders/light.hpp"
#include "player/arms.hpp"
#include "objects/skybox.hpp"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>



namespace imgui {
   void init(){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(Window::get_window(), true);
      ImGui_ImplOpenGL3_Init();
   }
   void frame(){
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
   }

   void main_draw(){
      ImGui::Begin("main window", 0, ImGuiWindowFlags_AlwaysAutoResize);
      {


         
         ImGui::SliderFloat("ZOOM OF CAMERA:", &Camera::get_instance()->zoom, -90.0f, 90.0f, "%.5f", 0);
         ImGui::SliderFloat3("POS OF CAMERA:", &Camera::get_instance()->pos.x, -100.0f, 100.0f, "%.5f", 0);
         ImGui::SliderFloat("NOISE INTENSITY:", &state.noise_intensity, 0.0f, 1.0f, "%.8f");
         ImGui::SliderFloat("CELL SIZE(LOW POLY):", &state.cell_size, 0.0f, 30.0f, "%.8f");
         ImGui::SliderFloat("FILTER THRESHOLD:", &state.filter_threshold, 0.0f, 1.0f, "%.8f");
         ImGui::ColorEdit4("LUMINANCE:", &state.filter_luminance_color.x);

         ImGui::ColorEdit4("FOG COLOR:", &Skybox::get_instance()->fog.color.x);
         ImGui::SliderFloat("FOG DENSITY:", &Skybox::get_instance()->fog.density, 0.0f, 1.0f, "%.8f");
         ImGui::SliderFloat("FOG START", &Skybox::get_instance()->fog.start, 0.0f, 2.0f, "%.8f");
         ImGui::SliderFloat("FOG END", &Skybox::get_instance()->fog.end, 0.0f, 2.0f, "%.8f");
         ImGui::SliderInt("FOG EQUATION:", &Skybox::get_instance()->fog.equation, 0, 2);

         ImGui::SliderFloat3("ARMS OFFSET:", &Arms::get_instance()->pos_offset.x, -10.0f, 10.0f);
         ImGui::SliderFloat3("ARMS SIZE:", &Arms::get_instance()->size.x, 0.0f, 10.0f);
         ImGui::SliderFloat("ARMS CAMERA FRONT OFFSET:", &Camera::get_instance()->front_offset, -5.0f, 5.0f);
         ImGui::SliderFloat("ARMS CAMERA RIGHT OFFSET:", &Camera::get_instance()->right_offset, -5.0f, 5.0f);
         ImGui::SliderFloat("ARMS CAMERA DOWN OFFSET:", &Camera::get_instance()->down_offset, -5.0f, 5.0f);


         // LIGHT
         ImGui::SeparatorText("LIGHT");
         light_t light = Light::get_instance()->get_flashlight();

         ImGui::ColorEdit3("color", &light.color.x);
         ImGui::SliderFloat("cut off", &light.cut_off, 0.0f, 180.0f); 
         ImGui::SliderFloat("outer cut off", &light.outer_cut_off, 0.0f, 180.0f);
         ImGui::SliderFloat("distance", &light.dist, 0.0f, 100.0f);
         ImGui::ColorEdit3("ambient", &light.ambient.x);
         ImGui::ColorEdit3("diffuse", &light.diffuse.x);
         ImGui::ColorEdit3("specular", &light.specular.x);
         ImGui::SliderFloat("constant", &light.constant, 0.0f, 10.0f);
         ImGui::SliderFloat("linear", &light.linear, 0.0f, 1.0f);
         ImGui::SliderFloat("quadratic", &light.quadratic, 0.0f, 1.0f);
        
         Light::get_instance()->flashlight = light;
         if (Camera::get_instance()->get_instance()->is_flying)
            ImGui::Text("FLYING ON");
         else
            ImGui::Text("FLYING OFF");
      }
      ImGui::End();
   }
   void render(){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   }
   void update_focused(){
      if (ImGui::IsWindowFocused() || ImGui::IsAnyItemFocused() ||
                            ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive()){
         state.global_state |= IMGUI_FOCUSED;
      } else {
         state.global_state ^= IMGUI_FOCUSED;
      }
   }


};

//                                 UTILS:                                 //

void draw_grid(ImDrawList* draw_list, ImVec2 origin, float cell_size, ImU32 color){
   int width, height;
   width = Window::get_width(), height = Window::get_height();
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

