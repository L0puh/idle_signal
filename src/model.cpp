#include <vector>
#include <assimp/material.h>
#include <glm/geometric.hpp>
#include <nlohmann/json.hpp>

#include "core.hpp"
#include "model.hpp"
#include "collision.hpp"
#include "light.hpp"


////////////////////////////////////////////////////////////////////////////
//                                MODEL                                   //
////////////////////////////////////////////////////////////////////////////

void Model::draw(){
   update();
   shd->use();
   shd->set_mat4fv("_projection", state.camera->get_projection());
   shd->set_mat4fv("_view", state.camera->get_view());
   shd->set_mat4fv("_model", model);
   if (!with_texture) {
      shd->set_vec3("_color", color);
   } else {
      state.light->set_all(shd);
   }

   for (uint i=0; i < meshes.size(); i++){
      meshes.at(i).draw();
   }
   shd->unuse();

}


void Model::draw_debug(){
   draw_debug(pos, size);
}
void Model::draw_debug(glm::vec3 pos, glm::vec3 size){

   for (int i = 0; i < meshes.size(); i++){
      for (int j = 0; j < meshes[i].vertices.size(); j+=2){
         glm::vec3 s, e;
         s = meshes[i].vertices[j].position;
         e = meshes[i].vertices[j+1].position;
         state.renderer->draw_line(s, e, color::red, 1.0f, state.resources->shaders[DEFAULT_SHADER], {pos, size});
      }
   }
}

void Model::load_model(const std::string src){
   const aiScene *scene;
   Assimp::Importer importer;
   
   scene = importer.ReadFile(src, ASSIMP_FLAGS_LOAD);
   if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      char info[64];
      sprintf(info, "assimp error: %s", importer.GetErrorString());
      log_info(info);
      error_and_exit(info);
      return;
   }
   char info[64];
   sprintf(info, "model is loaded: %s", src.c_str());

   process_node(scene->mRootNode, scene);
   if (with_texture) 
      shd = state.resources->shaders[TEXTURE_SHADER];
   else 
      shd = state.resources->shaders[DEFAULT_SHADER];

   if (shd == NULL) error_and_exit("error in init shader for model");


   log_info(info);
}

void Model::process_node(aiNode* node, const aiScene* scene){

   for (uint i=0; i < node->mNumMeshes; i++){
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(process_mesh(mesh, scene));
   }
   for (uint i=0; i < node->mNumChildren; i++){
      process_node(node->mChildren[i], scene);
   }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene){
   std::vector<data_t> verts;
   std::vector<uint> indices;
   std::vector<Texture> text;

   for (uint i = 0; i < mesh->mNumVertices; i++){
      data_t vert;
      vert.position = { mesh->mVertices[i].x, 
                        mesh->mVertices[i].y,
                        mesh->mVertices[i].z };
      if (mesh->HasNormals()){
         vert.normal= { mesh->mNormals[i].x, 
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z };
      }
      if (mesh->mTextureCoords[0]){
         with_texture = 1;
         vert.texcoord = {mesh->mTextureCoords[0][i].x,
                          mesh->mTextureCoords[0][i].y};
      } else {
         with_texture = 0;
         vert.texcoord = {0.0f, 0.0f};
      }
      verts.push_back(vert);
   }
   for (uint i = 0; i < mesh->mNumFaces; i++){
      aiFace face = mesh->mFaces[i];
      for (uint j = 0; j < face.mNumIndices; j++){
         indices.push_back(face.mIndices[j]);
      }
   }

   if (mesh->mMaterialIndex >= 0){
      aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> diffuse = load_texture(mat, aiTextureType_DIFFUSE, "diffuse");
      text.insert(text.end(), diffuse.begin(), diffuse.end());
      return Mesh(verts, indices, text);
   }

   return Mesh(verts, indices, text);
}

std::vector<Texture> Model::load_texture(aiMaterial *mat, aiTextureType type, std::string name){
   bool skip;
   aiString str;
   std::vector<Texture> texs;
   if (mat->GetTextureCount(type) > 0) with_texture = 1;
   else with_texture = 0;
   for (uint i=0; i < mat->GetTextureCount(type); i++){
      mat->GetTexture(type, i, &str);
      skip = false;
      for (uint j = 0; j < textures_loaded.size(); j++){
         if (std::strcmp(textures_loaded[j].get_name().c_str(), str.C_Str()) == 0){
            texs.push_back(textures_loaded[j]);
            skip=true;
            break;
         }
      }
      if (!skip){
         Texture tex(str.C_Str());
         tex.set_type(name);
         texs.push_back(tex);
         textures_loaded.push_back(tex);
      }
   }
   return texs;
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

////////////////////////////////////////////////////////////////////////////
//                                  MESH                                  //
////////////////////////////////////////////////////////////////////////////


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

void Vertex::setup_mesh(Mesh *mesh){
   create_VBO(&mesh->vertices[0], mesh->vertices.size() * sizeof(data_t));
   create_EBO(&mesh->indices[0], mesh->indices.size() * sizeof(uint));
   add_atrib(0, 3, GL_FLOAT, sizeof(data_t), 0);
   add_atrib(1, 3, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, normal)); 
   add_atrib(2, 2, GL_FLOAT, sizeof(data_t), (void*)offsetof(data_t, texcoord)); 
}
