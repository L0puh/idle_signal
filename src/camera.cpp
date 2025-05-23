#include "camera.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "model.hpp"
#include "collision.hpp"
#include "physics.hpp"
#include "terrain.hpp"


glm::mat4 Camera::get_projection_ortho() {
   float aspect = (float)window_width / (float)window_height;
   return glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height, 0.0f, 1.0f);
}

glm::mat4 Camera::get_projection(){
   glm::mat4 proj;
   float aspect = (float)window_width/window_height;
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
   if (input::is_pressed(window, GLFW_KEY_W)){
      p += front * vel; 
      is_walking = true;
   }
   if (input::is_pressed(window, GLFW_KEY_S)){
      p -= front * vel;
      is_walking = true;
   }
   if (input::is_pressed(window, GLFW_KEY_D)){
      p += right * vel;
      is_walking = true;
   }
   if (input::is_pressed(window, GLFW_KEY_A)){
      p -= right * vel;
      is_walking = true;
   }
   
   if (is_walking && !is_flying){
      walk_offset = sin(glfwGetTime() * 5.0f) * 0.01f;
      state.sound->play_sound(WALKING);
      
   } else { 
      walk_offset = 0.0f;
      state.sound->pause_sound(WALKING);
   }
   
   if (state.keys[GLFW_KEY_LEFT_SHIFT] && glfwGetTime() -
         state.keys_lastpress[GLFW_KEY_W] >= state.cooldown){
      if (speed <= 5.0f) 
         speed += 1.0 * state.deltatime;
   } else {
      speed = default_speed;
   }

   if (state.terrain->is_within(p, 20.0f)){
      this->pos = p;
      if (!is_flying)
         pos.y = state.terrain->get_height_at(pos.x, pos.z) + height/2.0f;
   }

   state.physics->perform_raycast_for_camera();
}


void Camera::update_movement2D(){
   if (input::is_pressed(window, GLFW_KEY_W)){
      pos.y += speed * state.deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_S)){
      pos.y -= speed * state.deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_D)){
      pos.x += speed * state.deltatime;
   }
   if (input::is_pressed(window, GLFW_KEY_A)){
      pos.x -= speed * state.deltatime;
   }
}

void Camera::init(){
   pos = {state.terrain->height/2.0f, 0.0f, state.terrain->width/2.0f};
   state.physics->set_camera_object();
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
   return from_ndc_to_world(from_screen_to_ndc({x,y}, size));
}

glm::vec2 Camera::get_mouse_pos() {
   double x, y;
   glfwGetCursorPos(window, &x, &y);
   return project(x, y, {window_height, window_width});
}
      
void Camera::update(){ 
   update_movement();
   view = glm::lookAt(pos + glm::vec3(0.0f, walk_offset + breathe_offset, 0.0f), pos+front, up);
   
   if (camera_bt != NULL)
      state.physics->update_camera_position();
}
