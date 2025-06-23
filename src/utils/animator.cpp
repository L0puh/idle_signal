#include "glm/ext/matrix_transform.hpp"
#include "utils/animation.hpp"
#include <string>


void Animator::clear_animations(){
   animations.clear();
   animations_counter = 0;
   current_animation = -1;
}

int Animator::add_animation(Skeletal_animation* animation){
   if (animation == NULL) error_and_exit("animation isn't loaded");
   animations[animations_counter].bone_models.reserve(100);
   for (int i = 0; i < 100; i++){
      animations[animations_counter].bone_models.push_back(glm::mat4(1.0f));
   }
   current_animation = animations_counter;
   animations_counter++;
   animations[current_animation].animation = animation;
   log_info("added new animation");
   return animations_counter-1;
}

void Animator::update_animation(int animation_id){
   if (animation_id >= animations_counter) error_and_exit("NO SUCH ANIMATION FOUND, ID EXCEEDS COUNTER. ADD ANIMATION FIRST");
   if (animation_id == -1) animation_id = current_animation;
   else current_animation = animation_id; 
   if (current_animation != -1){
      float cur_time = animations[animation_id].current_time;
      Skeletal_animation* current = animations[current_animation].animation;
      
      if (current == NULL) error_and_exit("ERROR LOADING ANIMATION, POINTER IS INCORRECT");
      cur_time += current->get_fps() * state.deltatime;
      cur_time = fmod(cur_time, current->get_duration());
      animations[animation_id].current_time = cur_time;
      calc_bone_transform(&current->get_root(), glm::mat4(1.0f));
   }
}

void Animator::set_transform(int id, glm::vec3 pos, glm::vec3 size){
   //TODO?
}

void Animator::set_current_animation(int id){
   if (id >= animations_counter) error_and_exit("NO SUCH ANIMATION FOUND, ID EXCEEDS COUNTER. ADD ANIMATION FIRST");
   current_animation = id;
}

void Animator::calc_bone_transform(const node_data_t* node, glm::mat4 trans){
   std::string name = node->name;
   glm::mat4 model = node->model;
   
   float cur_time = animations[current_animation].current_time;
   Skeletal_animation* current = animations[current_animation].animation;
  
   if (current == NULL) error_and_exit("ERROR LOADING ANIMATION, POINTER IS INCORRECT");
   
   Bone *bone = current->find_bone(name);
   if (bone) {
      bone->update(cur_time);
      model = bone->get_model();

   }
   glm::mat4 global = trans * model;
   auto info = current->get_bone_infos();
   if (info.find(name) != info.end()){
      int indx = info[name].id;
      glm::mat4 offset = info[name].offset;
      animations[current_animation].bone_models[indx] = global * offset;
   }
   for (int i = 0; i < node->children_cnt; i++){
      calc_bone_transform(&node->children[i], global);
   }
}


