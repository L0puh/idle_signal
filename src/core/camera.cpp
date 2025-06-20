
#include "core/core.hpp"
#include "audio/audio.hpp"
#include "core/camera.hpp"
#include "utils/input.hpp"
#include "core/window.hpp"
#include "physics/collision.hpp"

#include <glm/ext/matrix_clip_space.hpp>

void Camera::change_mode(uint8_t mode){
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
      pos = glm::vec3(Window::get_width()/2.0, 00.0f, Window::get_height()/2.0f);
      update_vectors();
      update();
   }
}
void Camera::hide_cursor() {
   input::hide_cursor();
   last_yaw = yaw;
   last_pitch = pitch;
}
void Camera::show_cursor() {
   input::show_cursor();
   pitch = last_pitch;
   yaw = last_yaw;
}

glm::mat4 Camera::get_projection_ortho() {
   float aspect = (float)Window::get_width()/ (float)Window::get_height();
   return glm::ortho(0.0f, (float)Window::get_width(), 0.0f, (float)Window::get_height(), 0.0f, 1.0f);
}

glm::mat4 Camera::get_projection(){
   glm::mat4 proj;
   float aspect = (float)Window::get_width()/Window::get_height();
   proj = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
   return proj;
      
}

bool Camera::is_pointing_to_object(glm::vec3& pos, float threshold){
   float prod, norm;
   glm::vec3 direction = pos - this->pos;
   
   norm = glm::length(direction);
   if (norm == 0) return false; 
   prod = glm::dot(glm::normalize(front), direction / norm);

   return prod > threshold;
}


void Camera::update_movement(){
   collider_t collider;
   glm::vec3 p = pos;
   float vel = speed * state.deltatime;

   is_walking = 0;
   if (input::is_pressed(GLFW_KEY_W)){
      p += front * vel; 
      is_walking = true;
   }
   if (input::is_pressed(GLFW_KEY_S)){
      p -= front * vel;
      is_walking = true;
   }
   if (input::is_pressed(GLFW_KEY_D)){
      p += right * vel;
      is_walking = true;
   }
   if (input::is_pressed(GLFW_KEY_A)){
      p -= right * vel;
      is_walking = true;
   }
   
   if (is_walking && !is_flying){
      walk_offset = sin(glfwGetTime() * 5.0f) * 0.01f;
      Sound::get_instance()->play_sound(WALKING);
      
   } else { 
      walk_offset = 0.0f;
      Sound::get_instance()->pause_sound(WALKING);
   }
   
   if (state.keys[GLFW_KEY_LEFT_SHIFT] && glfwGetTime() -
         state.keys_lastpress[GLFW_KEY_W] >= state.cooldown){
      if (speed <= 5.0f) 
         speed += 1.0 * state.deltatime;
   } else {
      speed = default_speed;
   }

   if (Terrain::get_instance()->is_within(p, 20.0f)){
      this->pos = p;
      if (!is_flying)
         pos.y = Terrain::get_instance()->get_height_at(pos.x, pos.z) + height/2.0f;
   }

   Physics::get_instance()->perform_raycast_for_camera();
}


void Camera::update_movement2D(){
   if (input::is_pressed(GLFW_KEY_W)){
      pos.y += speed * state.deltatime;
   }
   if (input::is_pressed(GLFW_KEY_S)){
      pos.y -= speed * state.deltatime;
   }
   if (input::is_pressed(GLFW_KEY_D)){
      pos.x += speed * state.deltatime;
   }
   if (input::is_pressed(GLFW_KEY_A)){
      pos.x -= speed * state.deltatime;
   }
}


void Camera::update_vectors(){
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   front = glm::normalize(front);
   right = glm::normalize(glm::cross(front, worldup));
   up = glm::normalize(glm::cross(right, front));
}
void Camera::update_mouse_turn(glm::vec2 offset){

   yaw += offset.x;
   pitch += offset.y;

   if (pitch > 89.0f) pitch = 89.0f; 
   if (pitch < -89.0f) pitch = -89.0f; 

   update_vectors();
}

glm::vec2 Camera::unproject(glm::vec3 pos, glm::vec2 screen_size){
    const auto clip = get_view() * get_projection() * glm::vec4{pos, 1.f};
    auto screen = glm::vec2(clip.x, clip.y) / clip.w;

    if (clip.w < 0.0f || screen.x >= 1.0f || screen.y >= 1.0f) {
        return {0.0f, 0.0f};
    }

    screen = (screen + glm::vec2{1.f, 1.f}) / 2.f;
    screen *= screen_size;
    return screen;
}
glm::vec2 Camera::project(double x, double y, glm::ivec2 size) {
   return Window::from_ndc_to_world(Window::from_screen_to_ndc({x,y}, size));
}

glm::vec2 Camera::get_mouse_pos() {
   double x, y;
   glfwGetCursorPos(Window::get_window(), &x, &y);
   return project(x, y, {Window::get_height(), Window::get_width()});
}
      
void Camera::update(){ 
   update_movement();
   view = glm::lookAt(pos + glm::vec3(0.0f, walk_offset + breathe_offset, 0.0f), pos+front, up);
   
   if (camera_bt != NULL)
      Physics::get_instance()->update_camera_position();
}
