#ifndef HAND_H
#define HAND_H

#include "camera.hpp"
#include "core.hpp"
#include "vertices.hpp"

#include <string>
#include <vector>


enum gun_action{
   FIRE,
   IDLE,
   WALK,
   RELOAD
};

class Hand{

   std::vector<Texture*> frames_gun_walk;
   std::vector<Texture*> frames_gun_idle;
   std::vector<Texture*> frames_gun_fire;
   std::vector<Texture*> frames_hand;

   Shader *blank;
   Vertex vert;
   int frame = 0;
   float last_change = 0.0f, framerate = 0.08f;;
   int count_vertices;
   int current_count_frames = 4;
   int gun_walk_frames = 30, gun_fire_frames = 9, hand_frames = 4;
   bool is_gun = 0;
   gun_action action = gun_action::WALK;
   public:
      Hand(){
         blank = new Shader(BLANK_SHADER_TEXTURE_VERT, BLANK_SHADER_TEXTURE_FRAG);
         vert.create_VBO(vertices::rectangle_with_texture, sizeof(vertices::rectangle_with_texture));
         vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
         vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); //pos
         vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float))); //tex
         count_vertices = LEN(vertices::rectangle_with_texture);
         
         for (int i = 1; i <= gun_walk_frames ; i++){
            std::string name="";
            name = "frames/WALKING/" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1);
            frames_gun_walk.push_back(tex);
         }
         for (int i = 1; i <= gun_fire_frames; i++){
            std::string name="";
            name = "frames/FIREING/" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1);
            frames_gun_fire.push_back(tex);
         }
         for (int i = 1; i <= hand_frames; i++){
            std::string name="";
            name = "hand" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1);
            frames_hand.push_back(tex);
         }
      }
      ~Hand() {
         for (int i = 0; i < hand_frames; i++){
            delete frames_hand[i];
         }
         for (int i = 0; i < gun_fire_frames; i++){
            delete frames_gun_fire[i];
         }
         for (int i = 0; i < gun_walk_frames; i++){
            delete frames_gun_walk[i];
         }
         delete blank;
      }
      void draw() {
         if (state.keys[GLFW_KEY_H] && glfwGetTime() - state.keys_lastpress[GLFW_KEY_H] >= state.cooldown){
            state.keys_lastpress[GLFW_KEY_H] = glfwGetTime();
            if (is_gun) current_count_frames = hand_frames;
            else current_count_frames = gun_walk_frames;
            is_gun = !is_gun;
            frame = 0;
         }
         if (state.keys[GLFW_MOUSE_BUTTON_LEFT] && glfwGetTime() - state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] >= state.cooldown && is_gun){
            state.keys_lastpress[GLFW_MOUSE_BUTTON_LEFT] = glfwGetTime();
            action = gun_action::FIRE;
            current_count_frames = gun_fire_frames;
            frame = 0;
            last_change = glfwGetTime();
         }
         blank->use();
         if (glfwGetTime() - last_change >= framerate){
            if (is_gun && action == WALK)
               frames_gun_walk[frame++]->use();
            else if (is_gun && action == FIRE)
               frames_gun_fire[frame++]->use();
            else frames_hand[frame++]->use();
            last_change = glfwGetTime();
         } else {
            if (is_gun && action == WALK)
               frames_gun_walk[frame]->use();
            else if (is_gun && action == FIRE)
               frames_gun_fire[frame]->use();
            else frames_hand[frame]->use();
         }
         if (frame == current_count_frames && (action != FIRE || !is_gun)) frame = 0;
         else if (frame == current_count_frames && action == FIRE) {
            action = WALK;
            frame = 0;
            current_count_frames = gun_walk_frames;
         }

         glm::mat4 model = glm::mat4(1.0f);
         if (is_gun){
            model = glm::translate(model, glm::vec3(0.3, -0.5 + state.camera->walk_offset, 0.0f) );
            model = glm::scale(model, glm::vec3(1.5f));
         } else {
            model = glm::translate(model, glm::vec3(0.7, -0.5 + state.camera->walk_offset, 0.0f) );
         }
         blank->set_mat4fv("_model", model);
         blank->set_float("_cell_size", state.cell_size);
         blank->set_float("_width", state.camera->window_width);
         blank->set_float("_height", state.camera->window_height);
         vert.draw_EBO(GL_TRIANGLES, count_vertices);
      
      }
};

#endif 
