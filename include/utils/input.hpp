#ifndef INPUT_H
#define INPUT_H

#include "core/core.hpp"
#include "core/window.hpp"
#include "core/camera.hpp"
#include "map/map.hpp"

namespace input {

   inline void hide_cursor() {
      glfwSetInputMode(Window::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   }
   inline void show_cursor() {
      glfwSetInputMode(Window::get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }
   inline bool is_pressed(int key) {
      return glfwGetKey(Window::get_window(), key) == GLFW_PRESS;
   }
   inline bool is_released(int key) {
      return glfwGetKey(Window::get_window(), key) == GLFW_RELEASE; 
   }
   inline void mouse_callback(GLFWwindow *window, int key, int action, int mods){
      if (action == GLFW_PRESS){
         state.keys[key]=1;
      }
      else if (action == GLFW_RELEASE) {
         state.keys[key]=0;
      }
      if (key == GLFW_MOUSE_BUTTON_LEFT) {
         if (action == GLFW_PRESS){
            if (!(state.global_state & MOUSE_CLICKED)) state.global_state |= MOUSE_CHANGED;
            state.global_state |= MOUSE_CLICKED;
         } else if (action == GLFW_RELEASE){
            if (state.global_state & MOUSE_CLICKED) state.global_state |= MOUSE_CHANGED;
            else state.global_state ^= MOUSE_CHANGED;;
            state.global_state ^= MOUSE_CLICKED;
         }
      }
   }
   inline void key_callback(GLFWwindow* window, int key, int code, int action, int mods){

      switch(key){
         case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            return;
         case GLFW_KEY_P:
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            if (!state.keys_lastpress[GLFW_KEY_P] || state.cooldown < glfwGetTime() - state.keys_lastpress[GLFW_KEY_P]){
               state.keys_lastpress[GLFW_KEY_P] = glfwGetTime(); 
               if (state.mode & PLAY_MODE){
                  state.mode = PAUSE_MODE;
               }
               else state.mode = PLAY_MODE;
            }
            break;
         case GLFW_KEY_F:
            // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
         case GLFW_KEY_G:
            if (!state.keys_lastpress[GLFW_KEY_G] || state.cooldown < glfwGetTime() - state.keys_lastpress[GLFW_KEY_G]){
               state.keys_lastpress[GLFW_KEY_G] = glfwGetTime(); 
               Camera::get_instance()->is_flying = !Camera::get_instance()->is_flying;
            }
            break;
         case GLFW_KEY_M:
            if (!state.keys_lastpress[GLFW_KEY_M] || state.cooldown < glfwGetTime() - state.keys_lastpress[GLFW_KEY_M]){
               if (state.mode & EDIT_MODE){
                  Map::get_instance()->generate_coords();
                  state.mode = PLAY_MODE;
                  Camera::get_instance()->change_mode(state.mode);
               }
               else if (state.mode & PLAY_MODE){
                  state.mode = EDIT_MODE;
                  Camera::get_instance()->change_mode(state.mode);
               }
               state.keys_lastpress[GLFW_KEY_M] = glfwGetTime(); 
            }
            break;
            
      }
      if (action == GLFW_PRESS) 
         state.keys[key] = true;
      else if (action == GLFW_RELEASE)
         state.keys[key] = false;
   }
   inline void cursor_callback(GLFWwindow* window, double xpos, double ypos){
      if (state.first_mouse) {
         state.last_mouse_pos = {xpos, ypos};
         state.first_mouse = false;
      }

      glm::vec2 offset, pos;
      pos = {xpos, ypos};
      offset.x = pos.x - state.last_mouse_pos.x;
      offset.y = state.last_mouse_pos.y - pos.y;
      state.last_mouse_pos = pos;
      offset *= state.mouse_sensitivity;
      Camera::get_instance()->update_mouse_turn(offset);
   }
};


#endif 
