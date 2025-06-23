#include "utils/animation.hpp"


Bone* Skeletal_animation::find_bone(const std::string& name){
   for (auto iter = bones.begin(); iter != bones.end(); iter++){
      if (iter->get_name() == name) return &(*iter);
   }
   return nullptr;
}

void Skeletal_animation::read_data(node_data_t& to, const aiNode* from){
   assert(from);

   to.name = from->mName.data;
   to.model = matrix_to_glm(from->mTransformation);
   to.children_cnt = from->mNumChildren;

   for (int i = 0; i < from->mNumChildren; i++)
   {
      node_data_t data;
      read_data(data, from->mChildren[i]);
      to.children.push_back(data);
   }
}
void Skeletal_animation::load_animation(const std::string& src, Model* model){
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(src, aiProcess_Triangulate);
   assert(scene && scene->mRootNode);
   auto animation = scene->mAnimations[0];
   duration = animation->mDuration;
   fps = animation->mTicksPerSecond;
   read_data(root, scene->mRootNode);
   read_missing_bones(animation, *model);
}

void Skeletal_animation::read_missing_bones(const aiAnimation* anim, Model& model){
   int size = anim->mNumChannels;
   auto& infos = model.get_bone_infos();
   int& bone_cnt = model.get_bone_cnt();
   
   for (int i = 0; i < size; i++){
      auto chan = anim->mChannels[i];
      std::string name = chan->mNodeName.data;
      if (infos.find(name) == infos.end()){
         infos[name].id = bone_cnt;
         bone_cnt++;
      }
      bones.push_back(Bone(name, infos[name].id, chan));
   }
   this->bone_infos = infos;
}


