#ifndef ANIMATION_H
#define ANIMATION_H 

#include "core/camera.hpp"
#include "core/core.hpp"
#include "objects/vertices.hpp"
#include "core/window.hpp"

#include <string>
#include <vector>
#include <string>
#include <sys/stat.h>

#define DIR_HAND_FRAMES "assets/textures/frames/hand/"
#define DIR_FLASHLIGHT_FRAMES "assets/textures/frames/flashlight/"

enum animation_type {
   HAND_ANIMATION,
   FLASHLIGHT
};

/*TODO: refactor, for now it's hardcoded */
class Animation{
   protected:
      static Animation *instance;
      Animation() {};
   public:
      static Animation *get_instance() { 
         if (instance == NULL) instance = new Animation();
         return instance; 
      }
   
   private:

      Shader *shd;
      Vertex vert;
     
      float last_change = 0.0f, framerate = 20/60.0f;;
      
      std::vector<Texture*> frames_flashlight;
      std::vector<Texture*> frames_hand;

      int frame = 0;
      int count_vertices;
      int count_frames_hand, count_frames_flashlight;
   

   public: 
      void init() {
         create_sprite_vertex();
         load_frames();
      }
      void cleanup(){
         for (int i = 0; i < count_frames_hand; i++){
            delete frames_hand[i];
         }
         for (int i = 0; i < count_frames_flashlight; i++){
            delete frames_flashlight[i];
         }
         delete shd;
      }
      void draw(animation_type type){
         float time = glfwGetTime();
         switch (type) {
            case HAND_ANIMATION:
               {
                  if (time - last_change >= framerate){
                     last_change = time;
                     frames_hand[frame++]->use();
                  } else {
                     frames_hand[frame]->use();
                  }
                  if (frame == count_frames_hand) frame = 0;
                  break;
               }
            case FLASHLIGHT:
            break;
         }
         glm::mat4 model(1.0f); 
         glm::vec3 p = (glm::vec3(Window::get_width() - 190.0f, Camera::get_instance()->walk_offset*2.0f + 200.0f, 0.0f));
         model = glm::translate(model, p);
         model = glm::scale(model, glm::vec3(450.0f));
         shd->use();
         shd->set_mat4fv("_model", Camera::get_instance()->get_projection_ortho() * model);
         shd->set_float("_cell_size", state.cell_size);
         shd->set_float("_width",  Window::get_width());
         shd->set_float("_height", Window::get_height());
         vert.draw_EBO(GL_TRIANGLES, count_vertices);
      }
   private:

      void load_frames(){
         std::string name;
         
         count_frames_flashlight = count_files(DIR_FLASHLIGHT_FRAMES);
         count_frames_hand = count_files(DIR_HAND_FRAMES);
         for (int i = 1; i <= count_frames_hand; i++){
            name = "frames/hand/" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1, 1);
            frames_hand.push_back(tex);
         }

         for (int i = 1; i <= count_frames_flashlight; i++){
            name = "frames/flashlight/" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1, 1);
            frames_hand.push_back(tex);
         }

      }

      void create_sprite_vertex(){
         shd = Resources::get_instance()->shaders[BLANK_SHADER]; 
         vert.create_VBO(vertices::rectangle_with_texture, sizeof(vertices::rectangle_with_texture));
         vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
         vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); 
         vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float))); 
         count_vertices = LEN(vertices::rectangle_with_texture);
      }

};

#endif 
