#include "camera.hpp"
#include "input.hpp"
#include "model.hpp"
#include "collision.hpp"

glm::mat4 Camera::get_projection_ortho() {
   glm::mat4 proj = glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height);
   return proj;
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
   } else walk_offset = 0.0f;

   collider = model->caclulate_boundaries();
   collider.pos = p;
   //TODO:: add sliding 
   if (!state.collision->AABB_collision_with(&collider) && !check_collision_with_walls(p))
      this->pos = p;
   
   if (!is_flying && pos.y != state.ground_level + 1.0f ) 
      pos.y = state.ground_level + 1.0f;
}

bool Camera::check_collision_with_walls(glm::vec3 p){
   for (const auto& wall: state.map->get_walls()){
      glm::vec3 min, max;
      min = glm::vec3(wall.first.x, state.ground_level, wall.first.z);
      max = wall.second;
      if (state.collision->line_circle(glm::vec2(min.x, min.z),
               glm::vec2(max.x, max.z), glm::vec2(p.x,
                  p.z), 0.4f))
      {
         return true;
      }
   }
   
   return false;
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

// world to screen positiong
glm::vec2 Camera::unproject(glm::vec3 pos){
   glm::vec4 position_clip = get_view() * get_projection()  * glm::vec4(pos, 1.0f) , position_screen;
   position_clip.z /= position_clip.w;
   position_clip.x /= position_clip.w;
   position_clip.y /= position_clip.w;
   

        // convert clip space position to screen space position
      glm::vec2 viewport = glm::vec2(window_width, window_height); 
        float viewport_half_width    = viewport.x* 0.5f;
        float viewport_half_height   = viewport.y * 0.5f;
        position_screen.x            = (position_clip.x / position_clip.z) *  viewport_half_width  + viewport_half_width;
        position_screen.y            = (position_clip.y / position_clip.z) * -viewport_half_height + viewport_half_height;
        return position_screen;
}


glm::vec2 Camera::project(double x, double y) {
   double normx, normy;
   normx = (2.0f * x) / window_width - 1.0f;
   normy = 1.0f - (2.0f * y) / window_height;
   glm::vec4 clip = glm::vec4(normx, normy, 1.0f, 1.0f); 
   glm::mat4 invproj = glm::inverse(get_projection());
   glm::vec4 world = invproj * clip;
   return world;
}

glm::vec2 Camera::get_mouse_pos() {
   double x, y;
   glfwGetCursorPos(window, &x, &y);
   return project(x, y);
}
      
void Camera::update(){ 
   update_movement();
   collision_obj->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), 
                                          btVector3(pos.x, pos.y, pos.z)));
   view = glm::lookAt(pos + glm::vec3(0.0f, walk_offset, 0.0f), pos+front, up);
   if (model != NULL) {
      model->set_pos(pos);
      model->set_size(size);
   }
}
