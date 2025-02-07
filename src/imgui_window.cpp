#include "core.hpp"

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
