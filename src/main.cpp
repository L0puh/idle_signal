#include "core.hpp"
#include "model.hpp"
#include <GLFW/glfw3.h>

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   Camera camera(window, 0);
   camera.set_pos(glm::vec3(1.0f));

   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;


   Model model("fish.obj");
   Shader shd;
   model.set_shader(&shd);
   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   while (!glfwWindowShouldClose(window)){

      imgui::frame();
      update_deltatime();
      camera.update();
      camera.hide_cursor();
      imgui::main_draw();
     
      model.set_pos(glm::vec3(1.0f));
      model.set_size(glm::vec3(0.004f, 0.004f, 0.004f));
      model.set_rotation(glfwGetTime(), {1.0f, 0.0f, 1.0f});
      model.draw();


      imgui::render();
      glfwSwapBuffers(window);
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
   
   shutdown(window);
   return 0;
}


void shutdown(GLFWwindow* window){
   glfwDestroyWindow(window);
   glfwTerminate();
   log_info("shutting down");
}

void enable_if_debug(){
#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(debug_message_callback, 0);
   log_info("debug mode is on");
#endif
}
