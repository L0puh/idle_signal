#include "player/arms.hpp"
#include "shaders/light.hpp"
#include "utils/resources.hpp"
#include "utils/animation.hpp"
#include "core/camera.hpp"
void Arms::init(){

   model = new Model(ARMS_MODEL_FILE, true);
   shd = Resources::get_instance()->shaders[ANIMATION_SHADER];
   
   animations.resize(ACTIONS_CNT);
   animations[IDLE]    = Animator::get_instance()->add_animation(new Skeletal_animation(ARMS_IDLE_FILE,    model));
   animations[PICKING] = Animator::get_instance()->add_animation(new Skeletal_animation(ARMS_PICKING_FILE, model));
   animations[HOLDING] = Animator::get_instance()->add_animation(new Skeletal_animation(ARMS_HOLDING_FILE, model));
   //...

}

void Arms::update_position(){

   glm::vec3 pos_on_screen = Camera::get_instance()->get_arms_position();
   glm::vec3 front    = {0.0, 0.0, 1.0f};
   glm::vec3 flat     = glm::normalize(glm::vec3(Camera::get_front().x, 0.0f, Camera::get_front().z));
   glm::vec3 rot      = glm::normalize(glm::cross(flat, Camera::get_up()));
   glm::vec3 point_to = Camera::get_pos() + Camera::get_front() * 10.0f;
   glm::vec3 axis     = glm::normalize(point_to - Camera::get_pos() + pos_on_screen);
   float horiz_angle  = atan2(axis.z, axis.x) - atan2(front.z, front.x);
   float vert_angle   = acos(glm::dot(flat, Camera::get_front()));
   if (horiz_angle < 0.0f) horiz_angle += 2.0f * M_PI; 
   if (Camera::get_front().y < 0.0f) vert_angle *= -1.0f;
   
   glm::mat4 m = glm::mat4(1.0f);
   m = translate(m, Camera::get_pos() - pos_offset);
   m = translate(m, -1.0f * axis * 0.02f);
   m = rotate(m, -horiz_angle, Camera::get_up());
   m = rotate(m, vert_angle, rot);
   m = scale(m, size);
   model->model = m;

   Animator::get_instance()->update_animation(animations[current_action]);
}
void Arms::update_action(){
    if (Camera::get_instance()->is_picked_object) current_action = HOLDING;
    else if (state.keys[GLFW_KEY_E] && glfwGetTime() - state.keys_lastpress[GLFW_KEY_E] > state.cooldown){
       current_action = PICKING;
    } else current_action = IDLE;

}

void Arms::draw(){
   update_position();
   
   shd->use();
   shd->set_mat4fv("_projection", Camera::get_instance()->get_projection());
   shd->set_mat4fv("_view", Camera::get_instance()->get_view());
   shd->set_mat4fv("_model", model->model);
   Light::get_instance()->set_all(shd);
   std::vector<glm::mat4> trans = Animator::get_instance()->get_bone_models(animations[current_action]);
   for (int i = 0; i < trans.size(); i++)
      shd->set_mat4fv("final[" + std::to_string(i) + "]", trans[i]);

   model->draw_meshes();
   shd->unuse();
}

