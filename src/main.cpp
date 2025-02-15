#include "core.hpp"
#include "model.hpp"

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

   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;


   Model model("cottage_blender.obj");
   Model model2("fish.obj");

   // Collision collision;
   // collision.add_collider(&model);
   // collision.add_collider(&model2);

   Shader shd;
   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   model.set_shader(&shd);
   model.set_size(glm::vec3(0.05f));

   model2.set_shader(&shd);
   model2.set_size(glm::vec3(0.005f));
   

   Renderer render;
   Shader shd2;
   shd2.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);


   collider_t a = model.caclulate_boundaries();
   collider_t b = model2.caclulate_boundaries();
   while (!glfwWindowShouldClose(window)){

      imgui::frame();
      update_deltatime();
      camera.update();
      camera.hide_cursor();
      imgui::main_draw();
      model.set_pos(glm::vec3(1.0f));
      model.draw();
      
      model2.set_pos(glm::vec3(2.0, 0.0, 0.0f));
      model2.set_rotation(glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));
      model2.draw();

      render.draw_line(b.max, b.min, color::red, 5.0f, &shd2, &model2);
      render.draw_cube(b.min, b.max, color::black, &shd2, &model2);
      render.draw_line(a.max, a.min, color::red, 5.0f, &shd2, &model);
      render.draw_cube(a.min, a.max, color::black, &shd2, &model);
      
      // collision.update_collisions();

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
