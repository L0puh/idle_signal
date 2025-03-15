#include "collision.hpp"
#include "core.hpp"
#include "model.hpp"
#include "camera.hpp"

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
   Model house("house.obj");
   Model monkey("monkey.obj");
   Model ball("ball.obj");
   Model plane_model("plane.obj");

   Texture text_tx;
   Shader shd, shd2, text_shader;
   Object text_obj(object_e::text, &text_tx, &text_shader);

   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shd2.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);

   house.set_shader(&shd);
   house.set_pos(glm::vec3(4.0, 0.0, 0.0f));
   house.set_size(glm::vec3(0.05f));

   monkey.set_shader(&shd);
   monkey.set_pos(glm::vec3(-2.0, 0.0, 0.0f));
   monkey.set_size(glm::vec3(0.05f));

   ball.set_shader(&shd);
   ball.set_pos(glm::vec3(-3.0f, -0.20f, 0.0f));
   ball.set_size(glm::vec3(0.05));

   plane_model.set_shader(&shd2);
   plane_model.set_color(color::green);
   plane_model.set_pos(glm::vec3(0.0f, -1.0f, 0.0));

   camera_model.set_shader(&shd);
   camera_model.set_size(camera.size);
   camera.set_model(&camera_model);

   Collision collision;
   collision.add_collider(&house);
   collision.add_collider(&monkey);
   // collision.add_collider(&ball);

   state.default_shader = &shd2;
   state.renderer = &render;
   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;
   state.collision = &collision;

   while (!glfwWindowShouldClose(window)){

      imgui::frame();
      update_deltatime();
     
      camera.update();
      camera.hide_cursor();

      imgui::main_draw();
      
      house.draw();
      monkey.draw();
      plane_model.draw();
      ball.draw();
      if (camera.is_close_to_object(ball.pos) && camera.is_pointing_to_object(ball.pos) && !ball.is_picked){
         render.draw_text(&text_obj, "PICK UP (E)", {state.camera->window_width/2.0f,
                     state.camera->window_height/2.0f - 40.0f }, 0.5, color::black);

         if (state.keys[GLFW_KEY_E])
            ball.is_picked = true;
      }

      if (state.keys[GLFW_KEY_E] && ball.is_picked){
         ball.set_pos(camera.pos - 0.1f);
      } else if (!state.keys[GLFW_KEY_E] && ball.is_picked){
         ball.is_picked = false;
      }
      render.draw_circle(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               0.4f, color::green, &shd2, {camera.pos + glm::vec3(0.0f,
               -camera.size.y, 0.0f), camera.size});
      
      render.draw_text(&text_obj, "+", {state.camera->window_width/2.0f,
                  state.camera->window_height/2.0f}, 0.5, color::white);
   
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
