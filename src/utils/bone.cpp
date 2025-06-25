#include "utils/animation.hpp"


void Bone::unpack_data(const aiNodeAnim* channel){
   n_pos = channel->mNumPositionKeys;
   for (int i = 0; i < n_pos; i++){
      key_pos_t data;
      aiVector3D ai_pos = channel->mPositionKeys[i].mValue;
      data.pos = glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z);
      data.time_stamp = channel->mPositionKeys[i].mTime;
      positions.push_back(data);
   }

   n_rots = channel->mNumRotationKeys;
   for (int i = 0; i < n_rots; ++i){
      key_rotation_t data;
      aiQuaternion ai_or = channel->mRotationKeys[i].mValue;
      data.time_stamp = channel->mRotationKeys[i].mTime;
      data.orient = glm::quat(ai_or.w, ai_or.x, ai_or.y, ai_or.z);
      rotations.push_back(data);

   }

   n_scales = channel->mNumScalingKeys;
   for (int i = 0; i < n_scales; i ++){
      key_scale_t data;
      aiVector3D scale = channel->mScalingKeys[i].mValue;
      data.scale = glm::vec3(scale.x, scale.y, scale.z);
      data.time_stamp = channel->mScalingKeys[i].mTime;
      scales.push_back(data);
   }
}

void Bone::update(float time){
   glm::mat4 trans  =  interpolate_pos(time);
   glm::mat4 rotate =  interpolate_rotation(time);
   glm::mat4 scale  =  interpolate_scale(time);

   model = trans * rotate * scale;
}


int Bone::get_pos_index(float time){
   for (int i = 0; i < n_pos-1; i++){
      if (time < positions[i+1].time_stamp) return i;
   }
   error_and_exit("Time isn't in the score of animation");
   return -1;
}
int Bone::get_scale_index(float time){
   for (int i = 0; i < n_scales-1; i++){
      if (time < scales[i+1].time_stamp) return i;
   }
   error_and_exit("Time isn't in the score of animation");
   return -1;
}
int Bone::get_rotation_index(float time){
   for (int i = 0; i < n_rots-1; i++){
      if (time < rotations[i+1].time_stamp) return i;
   }
   error_and_exit("Time isn't in the score of animation");
   return -1;
}

float Bone::get_scale_factor(float last_time_stamp, float next_time_stamp, float time){
   float mid = time - last_time_stamp;
   float diff = next_time_stamp - last_time_stamp;
   return mid / diff;
}
glm::mat4 Bone::interpolate_pos(float time){
   if (n_pos == 1) return glm::translate(glm::mat4(1.0f), positions[0].pos);
   int p1 = get_pos_index(time);
   int p2 = p1+1;
   float scale_factor = get_scale_factor(positions[p1].time_stamp,
                                         positions[p2].time_stamp, time);
   glm::vec3 pos = glm::mix(positions[p1].pos, positions[p2].pos, scale_factor);
   return glm::translate(glm::mat4(1.0f), pos);
   
}
glm::mat4 Bone::interpolate_rotation(float time){
   if (n_rots == 1)
      return glm::toMat4(glm::normalize(rotations[0].orient));

   int p1 = get_rotation_index(time);
   int p2 = p1 + 1;
   float scale_factor = get_scale_factor(rotations[p1].time_stamp,
                                         rotations[p2].time_stamp, time);
   glm::quat rotation = glm::slerp(rotations[p1].orient,
                                   rotations[p2].orient, scale_factor);
   return glm::toMat4(glm::normalize(rotation)); 
}

glm::mat4 Bone::interpolate_scale(float time){
   if (n_scales == 1)
         return glm::scale(glm::mat4(1.0f), scales[0].scale);

   int p1 = get_scale_index(time);
   int p2 = p1 + 1;
   float scale_factor = get_scale_factor(scales[p1].time_stamp,
                                         scales[p2].time_stamp, time);

   glm::vec3 scale = glm::mix(scales[p1].scale, 
                              scales[p2].scale,
                              scale_factor);

   return glm::scale(glm::mat4(1.0f), scale);
}
