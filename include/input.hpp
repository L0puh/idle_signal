#ifndef INPUT_H
#define INPUT_H

#include "state.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace input {
   inline bool is_pressed(GLFWwindow* window, int key) {
      return glfwGetKey(window, key) == GLFW_PRESS;
   }
   inline bool is_released(GLFWwindow *window, int key) {
      return glfwGetKey(window, key) == GLFW_RELEASE; 
   }
   inline void mouse_callback(GLFWwindow *window, int key, int action, int mods){
      if (action == GLFW_PRESS)
         state.keys[key]=1;
      else if (action == GLFW_RELEASE) 
         state.keys[key]=0;
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
      if (action == GLFW_PRESS) 
         state.keys[key] = true;
      else if (action == GLFW_RELEASE)
         state.keys[key] = false;

      switch(key){
         case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            return;
      }
   }
};


#endif 
