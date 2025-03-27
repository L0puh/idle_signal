#include "audio.hpp"
#include "collision.hpp"
#include "core.hpp"
#include "map.hpp"
#include "hand.hpp"
#include "state.hpp"
#include "vertices.hpp"
#include <vector>

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   memcpy(state.bg_color, color::dark_grey, 
         sizeof(color::dark_grey));
   state.light_color = {color::blue[0], color::blue[1], color::blue[2], 1.0f};

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

   camera_model.set_shader(&shd);
   camera_model.set_size(camera.size);
   camera.set_model(&camera_model);

   Collision collision;
   // collision.add_collider(&house);
   // collision.add_collider(&aircraft);
   // collision.add_collider(&ball);

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
   
   Shader shd_wall;
   Texture tex_wall("wall_texture.jpg"), tex_floor("floor_texture.jpg"), tex_roof("roof_texture.jpg");
   shd_wall.init_shader(WALL_SHADER_TEXTURE_VERT, WALL_SHADER_TEXTURE_FRAG);

   map.set_wall_texure(&tex_wall);
   map.set_floor_texture(&tex_floor);
   map.set_roof_texture(&tex_roof);
   map.set_shader(&shd_wall);

   state.light_pos = {0.1f, 0.1f, 0.1f};

   Hand hand;
   Texture cubemap;
   std::vector<std::string> faces = {
       "right.png",
       "left.png",
       "top.png",
       "bottom.png",
       "front.png",
       "back.png"
   };
   
   cubemap.load_cubemap(faces);
   Shader cubemap_shd;
   cubemap_shd.init_shader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
   Vertex vertex;
   vertex.create_VBO(vertices::skybox, sizeof(vertices::skybox));
   vertex.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float)); //pos

   Audio auidio;
   auidio.init();
   Sound sound;
   ALuint buf = auidio.add_sound_effect("assets/sounds/footsteps.ogg");

   while (!glfwWindowShouldClose(window)){

      glClearColor(state.bg_color[0], state.bg_color[1], state.bg_color[2], state.bg_color[3]);   
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      state.light_pos = ball.pos;
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

      if (camera.is_walking){
         sound.play_sound(buf);
      } else sound.pause_sound(buf);
      

      imgui::main_draw();
      const float c[] = {state.light_color[0], state.light_color[1], state.light_color[2],
         state.light_color[3]};
      render.draw_cube(state.light_pos, state.light_pos + glm::vec3(0.3, 0.3, 0.3), 
            c, state.default_shader, {glm::vec3(0.0f), glm::vec3(1.0f)});
      // world.update();
      house.draw();
      aircraft.draw();
      // ball.draw();
      


      for (auto& p: pickables){
         if (!camera.is_picked_object && camera.is_close_to_object(p->pos) 
                     && camera.is_pointing_to_object(p->pos) && !p->is_picked){
            render.draw_text(&text_obj, "PICK UP (E)", {state.camera->window_width/2.0f,
                        state.camera->window_height/2.0f - 40.0f }, 0.5, color::white);
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
  
      glDepthFunc(GL_LEQUAL);
      glm::mat4 model = glm::mat4(1.0f);
      model = camera.get_projection() * glm::mat4(glm::mat3(camera.get_view()));
      cubemap_shd.use();
      cubemap_shd.set_mat4fv("_model", model);
      vertex.bind();
      cubemap.use_cubemap();
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDepthFunc(GL_LESS);
      
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

