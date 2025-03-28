#include "audio.hpp"
#include "collision.hpp"
#include "core.hpp"
#include "map.hpp"
#include "hand.hpp"
#include "state.hpp"
#include "skybox.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   memcpy(state.bg_color, color::dark_grey, sizeof(color::dark_grey));
   state.light_color = {color::blue[0], color::blue[1], color::blue[2], 1.0f};

   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   
   Camera camera(window, 0);
   Renderer render;
   Collision collision;
   Hand hand;
   Audio audio;
   Sound sound;
   Map map;
   Texture text_tx;
   Shader texture_shd, default_shd, text_shader;
   Skybox skybox;
   Object text_obj(object_e::text, &text_tx, &text_shader);

   sound.init_sounds(&audio);
   state.default_shader = new Shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
   state.default_texture_shader = new Shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_FRAG); 
   state.renderer = &render;
   state.camera = &camera;
   state.collision = &collision;
   state.text_obj = &text_obj;
   state.sound = &sound;
   state.map = &map;
   state.mode |= PLAY_MODE;
   
   while (!glfwWindowShouldClose(window)){

      glClearColor(state.bg_color[0], state.bg_color[1], state.bg_color[2], state.bg_color[3]);   
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
      }
      
      imgui::main_draw();
     
      skybox.draw();
      hand.draw();
      
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

