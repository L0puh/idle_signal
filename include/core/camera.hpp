#ifndef CAMERA_HPP
#define CAMERA_HPP 

#include "core/core.hpp"
#include "physics/physics.hpp"
#include "objects/terrain.hpp"

#include <glm/glm.hpp>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

class Camera {
   protected:
      static Camera *instance;
      Camera(){}

   public:
      static Camera *get_instance() { 
         if (instance == NULL) instance = new Camera();
         return instance; 
      }

   public:

      btCollisionObject* camera_bt;
      bool is_picked_object = false;
      float walk_offset = 0.0f, breathe_offset = 0.0f;
      bool is_walking = false, is_colliding = false;
      uint8_t flags;
      glm::mat4 view;
      float default_speed = 3.4f;
      float speed, zoom;

      glm::mat4 last_view;
      bool is_flying = false;
      
      static glm::vec3 pos, size;
      static glm::vec3 front;
      static glm::vec3 worldup;
      static glm::vec3 up;
      
      glm::vec3 right;
      float yaw, pitch, height;
      float last_yaw, last_pitch;
      glm::vec3 last_pos;


   public:
      void init(uint8_t flags) {
         this->flags = flags;
         speed = default_speed;
         zoom = 45.0f, yaw = -90.0f, pitch = 0.0f; 
         height = 1.8f;
      }
      void setup_camera(){
         pos = {Terrain::get_height()/2.0f, 0.0f, Terrain::get_width()/2.0f};
         Physics::get_instance()->set_camera_object();
      }
      glm::vec2 unproject(glm::vec3 pos, glm::vec2 size);
      glm::vec2 project(double x, double y, glm::ivec2 size);
      
      glm::vec2 get_mouse_pos();
      glm::mat4 get_projection_ortho();
      glm::mat4 get_projection();
      void update();
      void update_mouse_turn(glm::vec2 offset);

   public:
      void hide_cursor();
      void show_cursor();
      
      void set_pos(glm::vec3 pos)   { this->pos   = pos; }
      void set_speed(float speed)   { this->speed = speed; }
      void set_zoom(float zoom)     { this->zoom  = zoom; }
      glm::mat4 get_view()          { return view;}
      void clear_flag(uint8_t flag) { flags &= ~flag; }
      void set_flag(uint8_t flag)   { flags |= flag; }
      void change_mode(uint8_t mode);
      static glm::vec3 get_pos()   { return pos; }
      static glm::vec3 get_size()  { return size; }
      static glm::vec3 get_front() { return front; }
      static glm::vec3 get_up()    { return up; }
      glm::vec3 get_right()        { return right; }

   public:
      glm::vec3 get_arms_position();
      bool is_pointing_to_object(glm::vec3& pos, float threshold=0.9f);
      inline bool is_close_to_object(glm::vec3& pos, float threshold=1.5f) {
         float dist = glm::length(this->pos - pos);
         return dist < threshold;
      }
   private:
      void update_vectors();
      void update_movement2D();
      void update_movement();

};

#endif 
