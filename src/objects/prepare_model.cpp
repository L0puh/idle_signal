#include "core/core.hpp"
#include "objects/model.hpp"
#include <fstream>

void Model::process_node(aiNode* node, const aiScene* scene){
   for (uint i=0; i < node->mNumMeshes; i++){
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(process_mesh(mesh, scene));
   }
   for (uint i=0; i < node->mNumChildren; i++){
      process_node(node->mChildren[i], scene);
   }
}

void Model::to_defaul_vertex_data(data_t& vert){
   for (int i = 0; i < MAX_BONE_WEIGHTS; i++){
      vert.bone_ids[i] = -1;
      vert.weights[i]  = 0.0f;
   }
}

//  MESH:  //

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene){
   std::vector<data_t> verts;
   std::vector<uint> indices;
   std::vector<Texture> textures;

   for (uint i = 0; i < mesh->mNumVertices; i++){
      data_t vert;
      to_defaul_vertex_data(vert); 
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

   aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
   std::vector<Texture> diffuse = load_texture(mat, aiTextureType_DIFFUSE, "diffuse", scene);
   textures.insert(textures.end(), diffuse.begin(), diffuse.end());
   
   if (with_animation && mesh->HasBones())
      extract_bones(verts, mesh, scene);
   else with_animation = 0;

   return Mesh(verts, indices, textures, with_animation);
}


void Model::extract_embedded_texture(aiMaterial* mat, const aiScene* scene, std::string* name){
   /* used for .glb mostly: extracts and saves to the folder with textures */

   log_info("extracing textures...");
   uint8_t indx = std::stoi(name->substr(1));
   const aiTexture* tex = scene->mTextures[indx];
   std::string ext = tex->achFormatHint;

   std::string filename = source_file + std::to_string(indx) + "." + ext;
   std::filesystem::path path = "assets/textures/" + filename;

   if (std::ofstream file(path, std::ios::binary); file.is_open()){
      if (tex->mHeight == 0){
          file.write(reinterpret_cast<const char*>(tex->pcData), tex->mWidth);
      } else error_and_exit("error in extracting textures");
   }
   *name = filename;
}

std::vector<Texture> Model::load_texture(aiMaterial *mat, aiTextureType type, std::string name, const aiScene* scene){
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
         std::string name = str.C_Str();
         if (name.front() == '*') extract_embedded_texture(mat, scene, &name);
         Texture tex(name);
         tex.set_type(name);
         texs.push_back(tex);
         textures_loaded.push_back(tex);
      }
   }
   return texs;
}
//  BONES: for skeletal animation //

void Model::set_vertex_bone(data_t& vert, int id, float weight){
   for (int i = 0; i < MAX_BONE_WEIGHTS; i++){
      if(vert.bone_ids[i] == -1) {
         vert.weights[i] = weight;
         vert.bone_ids[i] = id;
         break;
      }
   }
}

void Model::extract_bones(std::vector<data_t>& vertices, aiMesh* mesh, const aiScene* scene){
   auto& map = bone_infos;
   int& cnt = bone_cnt;
   for (int bone_indx = 0; bone_indx < mesh->mNumBones; bone_indx++){
      int bone_id = -1;
      std::string bone_name = mesh->mBones[bone_indx]->mName.C_Str();
      if (map.find(bone_name) == map.end()){
         map[bone_name].id     = cnt;
         map[bone_name].offset = matrix_to_glm(mesh->mBones[bone_indx]->mOffsetMatrix);
         bone_id = cnt;
         cnt++;
      } else {
         bone_id = map[bone_name].id;
      }
      assert(bone_id != -1);
      auto weights = mesh->mBones[bone_indx]->mWeights;
      int n_weights = mesh->mBones[bone_indx]->mNumWeights;

      for (int wi = 0; wi < n_weights; wi ++){
         int vert_id = weights[wi].mVertexId;
         assert(vert_id <= vertices.size());
         set_vertex_bone(vertices[vert_id], bone_id, weights[wi].mWeight);
      }

   }
}
