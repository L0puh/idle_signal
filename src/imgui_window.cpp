#include "core.hpp"
#include "camera.hpp"
#include "imgui/imgui.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace imgui {
   void init(GLFWwindow* window){
      ImGui::CreateContext();
      ImGuiIO& IO = ImGui::GetIO();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
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
         ImGui::SliderFloat("ZOOM OF CAMERA:", &state.camera->zoom, -90.0f, 90.0f, "%.5f", 0);
         ImGui::SliderFloat3("POS OF CAMERA:", &state.camera->pos.x, -100.0f, 100.0f, "%.5f", 0);
         ImGui::SliderFloat3("POS OF LIGHT:", &state.light_pos.x, -10.0f, 10.0f, "%.5f", 0);
         float color[4] = {state.light_color.x, state.light_color.y, state.light_color.z, state.light_color.w};
         float color2[4] = {state.filter_luminance_color.x, state.filter_luminance_color.y, state.filter_luminance_color.z,
         state.filter_luminance_color.z};
         ImGui::SliderFloat("NOISE INTENSITY:", &state.noise_intensity, 0.0f, 1.0f, "%.8f");
         ImGui::SliderFloat("CELL SIZE(LOW POLY):", &state.cell_size, 0.0f, 30.0f, "%.8f");
         ImGui::SliderFloat("FILTER THRESHOLD:", &state.filter_threshold, 0.0f, 1.0f, "%.8f");
         ImGui::ColorEdit4("LUMINANCE:", color2);
         ImGui::ColorEdit4("LIGHT:", color);
         state.light_color = {color[0], color[1], color[2], color[3]};
         state.filter_luminance_color = {color2[0], color2[1], color2[2], color2[3]};
         if (state.camera->is_flying)
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
