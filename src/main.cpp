#include "audio.hpp"
#include "core.hpp"
#include "map.hpp"
#include "light.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "state.hpp"
#include "skybox.hpp"
#include "physics.hpp"
#include "terrain.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   
   Physics physics;
   Light light;
   Resources resources;
   Skybox skybox;
   Camera camera(window, 0);
   Renderer renderer;
   Animation animation;
   Audio audio;
   Sound sound;
   Terrain terrain(400, 400);
   Map map;

   state.mode |= PLAY_MODE;
   sound.init_sounds(&audio);
   camera.init();

   //TEST LIGHT POINT
   glm::vec3 light_point(camera.pos);
   light.add_point_light(light_point);


   while (!glfwWindowShouldClose(window)){

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      imgui::frame();
      update_deltatime();
      skybox.draw();
      terrain.draw_terrain();

      renderer.draw_cube(glm::vec3(0.0f), glm::vec3(1.0f), color::blue, resources.shaders[DEFAULT_SHADER], {light_point, glm::vec3(1.0f)});
      

      if (state.mode & PLAY_MODE){
         camera.update();
         camera.hide_cursor();
         map.draw_objects();
      } else if (state.mode & EDIT_MODE)  {
         camera.show_cursor();
         map.editor_popup();
      } else if (state.mode & PAUSE_MODE) {
         camera.show_cursor();
         map.draw_objects();
         imgui::main_draw();
      }

      
      if (!camera.is_flying){
         physics.update_collisions();
         animation.draw(HAND_ANIMATION);
      }
      state.renderer->draw_text("+", {state.camera->window_width/2.0f,
               state.camera->window_height/2.0f}, 0.5, color::white);
      
      imgui::render();
      glfwSwapBuffers(window);
      glfwPollEvents();
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

