#include "core/core.hpp"
#include "audio/audio.hpp"
#include "map/map.hpp"
#include "shaders/light.hpp"
#include "utils/animation.hpp"
#include "utils/renderer.hpp"
#include "utils/resources.hpp"
#include "core/state.hpp"
#include "objects/skybox.hpp"
#include "physics/physics.hpp"
#include "objects/terrain.hpp"
#include "core/window.hpp"

void enable_if_debug();
void init_singletons();

STATE state;

/* declare singletons */
Window *Window::instance       = NULL;
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
Sound *Sound::instance         = NULL;

/* init static parameters */
GLFWwindow* Window::window = NULL;
int Window::height = 600;
int Window::width  = 600;
int Terrain::height = 400;
int Terrain::width= 400;
glm::vec3 Camera::front   = {0.0f, 0.0f, -1.0f};
glm::vec3 Camera::worldup = {0.0f, 1.0f, 0.0f};
glm::vec3 Camera::up      = {0.0f, 1.0f, 0.0f};
glm::vec3 Camera::size    = {0.1f, 0.2f, 0.1f};
glm::vec3 Camera::pos     = glm::vec3(0.0f);

int main() {
   Window::get_instance()->init_window(500, 500);
   imgui::init();
  
   enable_if_debug();
   init_singletons();
   state.mode |= PLAY_MODE;
   
   Map* map = Map::get_instance();
   Camera* camera = Camera::get_instance();
   
   camera->setup_camera();
   Sound::get_instance()->init_sounds();

   while (!glfwWindowShouldClose(Window::get_window())){

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      imgui::frame();
      Window::get_instance()->update_deltatime();
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

      Renderer::get_instance()->add_text({"+", {Window::get_width()/2.0f,
               Window::get_height()/2.0f}, 0.5, color::white});
      
      
      Renderer::get_instance()->draw_texts();
      imgui::render();
      glfwSwapBuffers(Window::get_window());
      glfwPollEvents();
   }
   
   Window::get_instance()->shutdown();
   return 0;
}

void init_singletons(){
   Physics::get_instance()->init_world();
   Light::get_instance()->init_flashlight();
   Resources::get_instance()->init();
   Skybox::get_instance()->init();
   Camera::get_instance()->init(0);
   Animation::get_instance()->init();
   Audio::get_instance()->init();
   Terrain::get_instance()->init(400, 400);
   Map::get_instance()->init();
   Sound::get_instance()->init();
}


void enable_if_debug(){
#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);

   glDebugMessageCallback(Window::debug_message_callback, 0);
   log_info("debug mode is on");
#endif
}

