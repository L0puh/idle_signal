#include "core.hpp"
#include "model.hpp"
#include "camera.hpp"

#include "collision.hpp"
#include "renderer.hpp"
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

   Renderer render;
   Model camera_model("camera.obj");
   Model model("fish.obj");


   Shader shd, shd2;
   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shd2.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);

   model.set_shader(&shd);
   model.set_pos(glm::vec3(2.0, -0.5, 0.0f));
   model.set_size(glm::vec3(0.005f));

   camera_model.set_shader(&shd);
   camera_model.set_size(camera.size);
   camera.set_model(&camera_model);

   Collision collision;
   collision.add_collider(&camera_model);
   collision.add_collider(&model);

   state.default_shader = &shd2;
   state.renderer = &render;
   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;

   while (!glfwWindowShouldClose(window)){

      imgui::frame();
      update_deltatime();
      camera.update();
      camera.hide_cursor();
      imgui::main_draw();
      
      model.draw();

      render.draw_circle(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f, color::green, &shd2, {camera.pos + glm::vec3(0.0f,
                  -camera.size.y, 0.0f), camera.size});
   
      collision.update_collisions();
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
