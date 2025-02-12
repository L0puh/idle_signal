#include "core.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "input.hpp"

glm::mat4 Camera::get_projection_ortho() {
   float aspect = (float)window_width/window_height;
   glm::mat4 proj = glm::ortho(-aspect * zoom, aspect*zoom, -aspect*zoom, aspect*zoom, -1.0f, 1.0f);
   return proj;
}

glm::mat4 Camera::get_projection(){
   glm::mat4 proj;
   float aspect = (float)window_width/window_height;
   proj = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
   return proj;
      
}

void Camera::update_movement(){
   float vel = speed * state.deltatime;
   if (input::is_pressed(window, GLFW_KEY_W)){
      pos += front * vel; 
   }
   if (input::is_pressed(window, GLFW_KEY_S)){
      pos -= front * vel;
   }
   if (input::is_pressed(window, GLFW_KEY_D)){
      pos += right * vel;
   }
   if (input::is_pressed(window, GLFW_KEY_A)){
      pos -= right * vel;
   }
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

glm::vec2 Camera::unproject(glm::vec2 pos){
    glm::vec4 world, screen;

    world = glm::vec4(pos, 1.0f, 1.0f);
    screen = get_projection() * get_view() * world;
    screen /= screen.w;
    screen.x = (screen.x + 1.0f) / 2.0f * window_width;
    screen.y = (screen.y + 1.0f) / 2.0f * window_height;

    return screen;
}

glm::vec2 Camera::project(double x, double y) {
   double normx, normy;
   normx = (x / window_width ) *  2.0f - 1.0f; 
   normy = (y / window_height) * -2.0f + 1.0f; 
   glm::mat4 proj = inverse(get_projection() * get_view());
   glm::vec4 world = glm::vec4(normx, normy, 1.0f, 1.0f) * proj;
   return world;
}
glm::vec2 Camera::get_mouse_pos() {
   double x, y;
   glfwGetCursorPos(window, &x, &y);
   return project(x, y);
}
      
void Camera::update(){ 
   update_movement();
   view = glm::lookAt(pos, pos+front, up);
}
