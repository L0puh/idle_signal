#include <vector>
#include <assimp/material.h>
#include <glm/geometric.hpp>
#include <nlohmann/json.hpp>

#include "core/core.hpp"
#include "utils/animation.hpp"
#include "core/camera.hpp"
#include "objects/model.hpp"
#include "physics/collision.hpp"
#include "shaders/light.hpp"



void Model::load_model(const std::string src){
   const aiScene *scene;
   Assimp::Importer importer;
   
   scene = importer.ReadFile(src, ASSIMP_FLAGS_LOAD);
   if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      Log::get_logger()->error("{} assimp error: {}", src, importer.GetErrorString());
      return;
   }
   process_node(scene->mRootNode, scene);
   
   if (with_animation) shd = Resources::get_instance()->shaders[ANIMATION_SHADER];
   else if (with_texture) shd = Resources::get_instance()->shaders[TEXTURE_SHADER];
   else shd = Resources::get_instance()->shaders[DEFAULT_SHADER];
   if (shd == NULL) 
         Log::get_logger()->error("error in init shader for model", src);

   if (with_animation){
      this->id_animation = Animator::get_instance()->add_animation(new Skeletal_animation(src, this));
   }
   Log::get_logger()->debug("{} is loaded", src);
}

void Model::draw_meshes(){
   for (uint i=0; i < meshes.size(); i++){
      meshes.at(i).draw();
   }
}


void Model::draw(){
   update();
   if (with_animation) Animator::get_instance()->update_animation(id_animation);
   
   shd->use();
   shd->set_mat4fv("_projection", Camera::get_instance()->get_projection());
   shd->set_mat4fv("_view", Camera::get_instance()->get_view());
   shd->set_mat4fv("_model", model);

   if (!with_texture && !with_animation) {
      shd->set_vec3("_color", color);
   } else {
      Light::get_instance()->set_all(shd);
   }

   if (with_animation){
      std::vector<glm::mat4> trans = Animator::get_instance()->get_bone_models(id_animation);
      for (int i = 0; i < trans.size(); i++){
         shd->set_mat4fv("final[" + std::to_string(i) + "]", trans[i]);
      }
   }
   draw_meshes();
   shd->unuse();
}


void Model::draw_debug(){
   draw_debug(pos, size);
}
void Model::draw_debug(glm::vec3 pos, glm::vec3 size){

   for (int i = 0; i < meshes.size(); i++){
      for (int j = 0; j < meshes[i].vertices.size()-1; j+=2){
         glm::vec3 s, e;
         s = meshes[i].vertices[j].position;
         e = meshes[i].vertices[j+1].position;
         Renderer::get_instance()->draw_line(s, e, color::red, 1.0f, {pos, size});
      }
   }
}


collider_t Model::caclulate_boundaries(){
   float maxlen = 0.0f;
   glm::vec3 min = glm::vec3(10000000000000.f), max = glm::vec3(0.0f);

   for (auto m = meshes.begin(); m != meshes.end(); m++){
      for (auto v = m->vertices.begin(); v != m->vertices.end(); v++){
         float len = glm::length(v->position);
         if (len > maxlen) {
            maxlen = len;
         }
         if (v->position.x < min.x) min.x = v->position.x;
         if (v->position.y < min.y) min.y = v->position.y;
         if (v->position.z < min.z) min.z = v->position.z;

         if (v->position.x > max.x) max.x = v->position.x;
         if (v->position.y > max.y) max.y = v->position.y;
         if (v->position.z > max.z) max.z = v->position.z;

      }
   }
   
   collider_t collider;
   collider.min = min;
   collider.max = max;
   collider.pos = this->pos;
   collider.size = this->size;
   collider.model = this;
   return collider;
}


void Mesh::draw(){
   for (uint i = 0; i < textures.size(); i++){
      textures.at(i).use();
   }
   vertex.bind();
   vertex.draw_EBO(GL_TRIANGLES, indices.size());
   vertex.unbind();
   if (textures.size() > 0)    
      textures.at(0).unuse();
}

