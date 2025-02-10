#include "core.hpp"
#include "glm/ext/matrix_transform.hpp"
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

   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;


   Model model("fish.obj");
   
   Shader shd;
   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   while (!glfwWindowShouldClose(window)){
      imgui::frame();
      update_deltatime();
      camera.update();
      imgui::main_draw();
     
      glm::mat4 mod = glm::mat4(1.0f);
      mod = glm::translate(mod, {0.0f, 0.0f, -4.0f});
      mod = glm::scale(mod, {0.004f, 0.004f, 0.004f});
      mod = glm::rotate(mod, (float)glfwGetTime(), {0.0f, 0.0f, 0.3f});
   
      shd.use();
      shd.set_mat4fv("_projection", state.camera->get_projection());
      shd.set_mat4fv("_view", state.camera->get_view());
      shd.set_mat4fv("_model", mod);
      model.draw();
      shd.unuse();


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
