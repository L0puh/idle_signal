#ifndef CAMERA_HPP
#define CAMERA_HPP 

#include <glm/glm.hpp>
#include "core.hpp"

class Camera {
   public:
      float walk_offset = 0.0f;
      bool is_walking = false;
      Model *model;
      GLFWwindow* window;
      uint8_t flags;
      glm::mat4 view;
      glm::vec3 pos, size;
      float speed, zoom;
      double window_width, window_height;

      float ground_level = 0.0f;
      bool is_flying = false;

      glm::vec3 front   = {0.0f, 0.0f, -1.0f};
      glm::vec3 worldup = {0.0f, 1.0f, 0.0f};
      glm::vec3 up      = {0.0f, 1.0f, 0.0f};
      glm::vec3 right;

      float yaw, pitch;
      
   public:
      Camera(GLFWwindow* window, uint8_t flags): window(window),
         speed(1.5f), flags(flags), 
         zoom(45.0f), yaw(-90.0f), pitch(0.0f), size(0.1f, 0.2f, 0.1f){}
   public:
      glm::vec2 unproject(glm::vec2 pos);
      glm::vec2 project(double x, double y);
      
      glm::vec2 get_mouse_pos();
      glm::mat4 get_projection_ortho();
      glm::mat4 get_projection();
      void update();
      void update_mouse_turn(glm::vec2 offset);

   public:
      void hide_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      void show_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      void set_pos(glm::vec3 pos)   { this->pos   = pos; }
      void set_speed(float speed)   { this->speed = speed; }
      void set_zoom(float zoom)     { this->zoom  = zoom; }
      glm::vec2 get_window_size()   { return glm::vec2(window_width, window_height); }
      glm::mat4 get_view()          { return view;}
      void clear_flag(uint8_t flag) { flags &= ~flag; }
      void set_flag(uint8_t flag)   { flags |= flag; }
      void set_model(Model* model)  { this->model = model; }

   private:
      void update_vectors();
      void update_movement2D();
      void update_movement();

};

#endif 
