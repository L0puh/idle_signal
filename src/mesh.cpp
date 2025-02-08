#include "core.hpp"
#include <vector>


void Mesh::draw(){
   // for (uint i = 0; i < textures.size(); i++){
      //TODO: get texture
   // }
   vertex.bind();
   vertex.draw_EBO(GL_TRIANGLES, indices.size());
   vertex.unbind();
}

void Model::load_model(const std::string src){
   const aiScene *scene;
   Assimp::Importer importer;

   scene = importer.ReadFile(src, aiProcess_Triangulate | aiProcess_FlipUVs);
   if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      //FIXME 
      printf("assimp error: %s\n", importer.GetErrorString());
      return;
   }
   char info[64];
   sprintf(info, "model is loaded: %s", src.c_str());
   log_info(info);

   process_node(scene->mRootNode, scene);
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
         //TODO
         // log_info("MESH WITH TEXTURE\n");
         vert.texcoord = {0.0f, 0.0f};
      } else {
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
   return Mesh(verts, indices, text);
}
