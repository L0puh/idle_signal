#include "audio.hpp"
#include "core.hpp"
#include "map.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "state.hpp"
#include "skybox.hpp"
#include "physics.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   
   Physics physics;
   Camera camera(window, 0);
   Renderer render;
   Animation animation;
   Audio audio;
   Sound sound;
   Map map;
   Texture text_tx;
   Shader texture_shd, default_shd, text_shader;
   Skybox skybox;
   Resources resources;
   Object text_obj(object_e::text, &text_tx, &text_shader);

   state.physics = &physics;
   state.default_shader = new Shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
   state.default_texture_shader = new Shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG); 
   state.renderer = &render;

   state.resources = &resources;
   state.camera = &camera;
   state.text_obj = &text_obj;
   state.sound = &sound;
   state.map = &map;
   state.mode |= PLAY_MODE;
   state.light_pos = {0.0f, 2.0f, 0.0};
   state.light_color = {color::blue[0], color::blue[1], color::blue[2], 1.0f};
   
   resources.init_models();
   sound.init_sounds(&audio);
   camera.init();

   while (!glfwWindowShouldClose(window)){

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      imgui::frame();
      update_deltatime();
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
      
      physics.update_collisions();
      skybox.draw();
      animation.draw(HAND_ANIMATION);
      
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

