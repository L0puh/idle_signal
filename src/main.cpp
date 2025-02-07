#include "core.hpp"
#include "object.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   Camera camera(window, 0);

   Object cube(object_e::cube);
   cube.set_pos(glm::vec3(0.0f, 0.0f, -8.0f));
   cube.set_color(color::green);
   cube.set_size(glm::vec3(1.0f, 1.0f, 1.0f));

   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;
   while (!glfwWindowShouldClose(window)){
      imgui::frame();
      update_deltatime();
      camera.update();
      imgui::render();
      
      cube.set_rotation((float)glfwGetTime() * glm::radians(40.0f), glm::vec3(1.0f, 0.3f, 1.3f));
      cube.draw_object();

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
