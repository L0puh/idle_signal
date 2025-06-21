#include "utils/animation.hpp"


Animator::Animator(Skeletal_animation *anim):
            cur_time(0.0f), cur_anim(anim) 
{
      
   bone_models.reserve(100);
   for (int i = 0; i < 100; i++){
      bone_models.push_back(glm::mat4(1.0f));
   }
}	

void Animator::update_animation(float dt){
   deltatime = dt;
   if (cur_anim){
      cur_time += cur_anim->get_fps() * dt;
      cur_time = fmod(cur_time, cur_anim->get_duration());
      calc_bone_transform(&cur_anim->get_root(), glm::mat4(1.0f));
      
   }
}

void Animator::play_animation(Skeletal_animation* anim){
   cur_anim = anim;
   cur_time = 0.0f;
}

void Animator::calc_bone_transform(const node_data_t* node, glm::mat4 trans){
   std::string name = node->name;
   glm::mat4 model = node->model;
   Bone *bone = cur_anim->find_bone(name);
   if (bone) {
      bone->update(cur_time);
      model = bone->get_model();
   }
   glm::mat4 global = trans * model;
   auto info = cur_anim->get_bone_infos();
   if (info.find(name) != info.end()){
      int indx = info[name].id;
      glm::mat4 offset = info[name].offset;
      bone_models[indx] = global * offset;
   }

   for (int i = 0; i < node->children_cnt; i++){
      calc_bone_transform(&node->children[i], global);
   }
}


