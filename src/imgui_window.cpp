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
         ImGui::SliderFloat3("POS OF CAMERA:", &state.camera->pos.x, -10.0f, 10.0f, "%.5f", 0);
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
