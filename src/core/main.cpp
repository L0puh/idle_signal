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
void init_singletons(GLFWwindow*);


/* declare singletons */
Physics *Physics::instance     = NULL;
Light *Light::instance         = NULL;
Resources *Resources::instance = NULL; 
Skybox *Skybox::instance       = NULL; 
Camera *Camera::instance       = NULL; 
Renderer *Renderer::instance   = NULL; 
Animation *Animation::instance = NULL; 
Audio *Audio::instance         = NULL; 
Terrain *Terrain::instance     = NULL; 
Map *Map::instance             = NULL; 


int main() {
   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   init_singletons(window);
   Sound sound;
   state.mode |= PLAY_MODE;
   sound.init_sounds();
   Camera* camera = Camera::get_instance();
   camera->setup_camera();
   Map* map = Map::get_instance();

   while (!glfwWindowShouldClose(window)){

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      imgui::frame();
      update_deltatime();
      Skybox::get_instance()->draw();
      Terrain::get_instance()->draw_terrain();

      if (state.mode & PLAY_MODE){
         camera->update();
         camera->hide_cursor();
         map->draw_objects_on_terrain();
      } else if (state.mode & EDIT_MODE)  {
         camera->show_cursor();
         map->editor_popup();
      } else if (state.mode & PAUSE_MODE) {
         camera->show_cursor();
         map->draw_objects_on_terrain();
         imgui::main_draw();
      }

      
      if (!camera->is_flying){
         Physics::get_instance()->update_collisions();
         Animation::get_instance()->draw(HAND_ANIMATION);
      }
      Renderer::get_instance()->add_text({"+", {Camera::get_instance()->window_width/2.0f,
               Camera::get_instance()->window_height/2.0f}, 0.5, color::white});
      
      
      Renderer::get_instance()->draw_texts();
      imgui::render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   
   shutdown(window);
   return 0;
}

void init_singletons(GLFWwindow *window){
   Physics::get_instance()->init_world();
   Light::get_instance()->init_flashlight();
   Resources::get_instance()->init();
   Skybox::get_instance()->init();
   Camera::get_instance()->init(window, 0);
   Animation::get_instance()->init();
   Audio::get_instance()->init();
   Terrain::get_instance()->init(400, 400);
   Map::get_instance()->init();
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

