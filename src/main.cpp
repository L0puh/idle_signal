#include <btBulletDynamicsCommon.h>
#include "collision.hpp"
#include "core.hpp"
#include "map.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);


int main() {
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   
   World world;
   state.world = &world;
   
   Camera camera(window, 0);
   camera.set_pos(glm::vec3(1.0f));

   Renderer render;
   Model camera_model("camera.obj");
   Model house("tower.obj");
   Model aircraft("aircraft.obj");
   Model ball("ball.obj");
   Model plane_model("plane.obj");

   Texture text_tx;
   Shader shd, shd2, text_shader;
   Object text_obj(object_e::text, &text_tx, &text_shader);

   shd.init_shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shd2.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);

   house.set_shader(&shd);
   house.set_pos(glm::vec3(4.0, -1.0, 0.0f));
   house.set_size(glm::vec3(0.4f));
   world.add_collision_object(house.collision_shape, house.pos);

   aircraft.set_shader(&shd);
   aircraft.set_pos(glm::vec3(-2.0, 0.0, 0.0f));
   aircraft.set_size(glm::vec3(0.05f));

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
   // collision.add_collider(&house);
   collision.add_collider(&aircraft);
   collision.add_collider(&ball);

   state.default_shader = &shd2;
   state.renderer = &render;
   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;
   state.collision = &collision;
      
   std::vector<Model*> pickables;
   pickables.push_back(&aircraft);
   pickables.push_back(&ball);

   Map map;
   state.mode |= PLAY_MODE;
   state.map = &map;
   
   Texture tex_wall("wall_texture.jpg");
   Shader shd_wall;
   shd_wall.init_shader(WALL_SHADER_TEXTURE_VERT, WALL_SHADER_TEXTURE_FRAG);

   while (!glfwWindowShouldClose(window)){
      imgui::frame();
      update_deltatime();
      if (state.mode & PLAY_MODE){
         camera.update();
         camera.hide_cursor();
      } else {
         camera.show_cursor();
         map.editor_popup();
      }

      imgui::main_draw();
      world.update();

      for (const auto& wall: map.walls_obj){
         Object w(object_e::wall, &tex_wall, &shd_wall, 
               {wall.first.x, -1.0f, wall.first.z}, wall.second);

         w.set_pos(glm::vec3(0.0f));
         w.set_size(glm::vec3(1.0f));
         w.draw();

      }
      for (const auto& floor: map.floors_obj){
         render.draw_rectangle(floor.second, floor.first, color::red,
               state.default_shader, {glm::vec3(0.0f),
               glm::vec3(1.0f)});

      }
      house.draw();
      aircraft.draw();
      plane_model.draw();
      ball.draw();
      for (auto& p: pickables){
         if (!camera.is_picked_object && camera.is_close_to_object(p->pos) 
                     && camera.is_pointing_to_object(p->pos) && !p->is_picked){
            render.draw_text(&text_obj, "PICK UP (E)", {state.camera->window_width/2.0f,
                        state.camera->window_height/2.0f - 40.0f }, 0.5, color::black);
            if (state.keys[GLFW_KEY_E]){
               p->is_picked = true;
               camera.is_picked_object = true;
            }
            break;
         }

         if (state.keys[GLFW_KEY_E] && p->is_picked){
            p->set_pos(camera.pos + camera.front);
         } else if (!state.keys[GLFW_KEY_E] && p->is_picked){
            p->set_pos(camera.pos + camera.front * 1.04f);
            p->is_picked = false;
            camera.is_picked_object = false;
         }
      }
      render.draw_circle(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               0.4f, color::red, &shd2, {camera.pos + glm::vec3(0.0f,
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
