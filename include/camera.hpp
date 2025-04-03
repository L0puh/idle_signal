#ifndef CAMERA_HPP
#define CAMERA_HPP 

#include "map.hpp"
#include "core.hpp"

#include <glm/glm.hpp>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

class Camera {
   public:

      btCollisionObject* camera_bt;
      bool is_picked_object = false;
      float walk_offset = 0.0f;
      bool is_walking = false, is_colliding = false;
      GLFWwindow* window;
      uint8_t flags;
      glm::mat4 view;
      glm::vec3 pos, size;
      float default_speed = 3.4f;
      float speed, zoom;
      double window_width, window_height;

      
      glm::mat4 last_view;
      bool is_flying = false;

      glm::vec3 front   = {0.0f, 0.0f, -1.0f};
      glm::vec3 worldup = {0.0f, 1.0f, 0.0f};
      glm::vec3 up      = {0.0f, 1.0f, 0.0f};
      glm::vec3 right;

      float yaw, pitch, height;


      float last_yaw, last_pitch;
      glm::vec3 last_pos;

   public:
      Camera(GLFWwindow* window, uint8_t flags): window(window),
         speed(default_speed), flags(flags), 
         zoom(45.0f), yaw(-90.0f), pitch(0.0f), size(0.1f, 0.2f, 0.1f),
         pos(glm::vec3(0.0f)), height(1.8f)
      {
         state.camera = this;
      }

   public:
      
      glm::vec2 unproject(glm::vec3 pos);
      glm::vec2 project(double x, double y);
      
      glm::vec2 get_mouse_pos();
      glm::mat4 get_projection_ortho();
      glm::mat4 get_projection();
      void update();
      void update_mouse_turn(glm::vec2 offset);

   public:
      void hide_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         last_yaw = yaw;
         last_pitch = pitch;
      }
      void show_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         pitch = last_pitch;
         yaw = last_yaw;
      }
      
      void set_pos(glm::vec3 pos)   { this->pos   = pos; }
      void set_speed(float speed)   { this->speed = speed; }
      void set_zoom(float zoom)     { this->zoom  = zoom; }
      glm::vec2 get_window_size()   { return glm::vec2(window_width, window_height); }
      glm::mat4 get_view()          { return view;}
      void clear_flag(uint8_t flag) { flags &= ~flag; }
      void set_flag(uint8_t flag)   { flags |= flag; }

      void change_mode(uint8_t mode){
         if (mode & PLAY_MODE){
            pos = last_pos;
            pitch = last_pitch;
            yaw = last_yaw;
            is_flying = false;
         } 
         if (mode & EDIT_MODE){
            last_pos = pos;
            last_yaw = yaw;
            last_pitch = pitch;
            is_flying = true;
            yaw = 0.0f;
            pitch = -90.0f;
            pos = glm::vec3(state.map->offset, 20.0f, state.map->offset);
            update_vectors();
            update();
         }
      }
   public:
      bool check_collision_with_walls(glm::vec3 pos);
      bool is_pointing_to_object(glm::vec3& pos, float threshold=0.9f);
      inline bool is_close_to_object(glm::vec3& pos, float threshold=1.5f) {
         float dist = glm::length(this->pos - pos);
         return dist < threshold;
      }
      void init();
   private:
      void update_vectors();
      void update_movement2D();
      void update_movement();

};

#endif 
