#include "player/arms.hpp"
#include "glm/geometric.hpp"
#include "shaders/light.hpp"
#include "utils/resources.hpp"
#include "utils/animation.hpp"

void Arms::init(){
   log_info("CREATE ARMS...");

   model = new Model(ARMS_MODEL_FILE, true);
   shd = Resources::get_instance()->shaders[ANIMATION_SHADER];
   
   animations.resize(ACTIONS_CNT);
   animations[IDLE] = Animator::get_instance()->add_animation(new Skeletal_animation(ARMS_IDLE_FILE, model));
   animations[PICKING] = Animator::get_instance()->add_animation(new Skeletal_animation(ARMS_PICKING_FILE, model));
   //...

}
void Arms::update_action(){
   //TODO: logic for changing actions....

   glm::vec3 pos_on_screen = Camera::get_instance()->get_arms_position();
   glm::vec3 front = {0.0, 0.0, 1.0f};
   glm::vec3 flat = glm::normalize(glm::vec3(Camera::get_front().x, 0.0, Camera::get_front().z));
   glm::vec3 rot  = glm::normalize(glm::cross(flat, Camera::get_up()));
   glm::vec3 point_to = Camera::get_pos() + Camera::get_front() * 20.0f;
   glm::vec3 axis = glm::normalize(point_to - Camera::get_pos() + pos_on_screen);
   float horiz_angle = atan2(axis.z, axis.x) - atan2(front.z, front.x);
   float vert_angle  = acos(glm::dot(flat, Camera::get_front()));
   if (horiz_angle < 0.0f) horiz_angle += 2.0f * M_PI; 
   if (Camera::get_front().y < 0.0f) vert_angle *= -1.0f;
   

   model->model = glm::mat4(1.0f);
   model->model = translate(model->model, Camera::get_pos() - glm::vec3(0.0, 1.8f, 0.0));
   model->model = translate(model->model, -1.0f * axis * 0.02f);
   model->model = scale(model->model, size);
   model->model = rotate(model->model, -horiz_angle, Camera::get_up());
   model->model = rotate(model->model, vert_angle, rot);
   Animator::get_instance()->update_animation(animations[current_action]);
}

void Arms::draw(){
   shd->use();
   shd->set_mat4fv("_projection", Camera::get_instance()->get_projection());
   glm::mat4 view = Camera::get_instance()->get_view();
   shd->set_mat4fv("_view", view);
   shd->set_mat4fv("_model", model->model);
   Light::get_instance()->set_all(shd);

   std::vector<glm::mat4> trans = Animator::get_instance()->get_bone_models(animations[current_action]);
   for (int i = 0; i < trans.size(); i++)
      shd->set_mat4fv("final[" + std::to_string(i) + "]", trans[i]);

   model->draw_meshes();
   shd->unuse();
}

