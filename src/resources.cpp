#include "resources.hpp"
#include "core.hpp"
#include "state.hpp"
#include "object.hpp"

#include <string>


void Resources::init_models_counter(std::string filename, models_type type) {
   std::string path = "assets/models/" + filename;
   int cnt = count_files(path)/2; // don't count .mtl
   for (int i = 1; i <= cnt; i++){
      std::string name = filename + "/" + std::to_string(i) + ".obj";
      models[type].push_back(new Model(name));
   }
}
void Resources::init_models(){
   init_models_counter("rocks", ROCK);
   init_models_counter("buildings", BUILDING);
   init_models_counter("bushes", BUSH);
   init_models_counter("trees", TREE);
}

void Resources::init_text(){ 
   if (textures[TEXT_TEXTURE] == NULL || shaders[TEXT_SHADER] == NULL){
      error_and_exit("init text shader and texture first");
   }
   text_obj = new Object(object_e::text, textures[TEXT_TEXTURE], shaders[TEXT_SHADER]);
   log_info("init text done");
}

void Resources::load_textures(){
   textures[TERRAIN_TEXTURE] = new Texture("terrain.jpg");
   textures[CUBEMAP_TEXTURE] = new Texture(cubemap_faces);
   textures[TEXT_TEXTURE] = new Texture;
   log_info("textures init done");
}

void Resources::load_shaders(){
   shaders[DEFAULT_SHADER] = new Shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
   shaders[TEXTURE_SHADER] = new Shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shaders[MAP_SHADER] = new Shader(MAP_SHADER_TEXTURE_VERT, MAP_SHADER_TEXTURE_FRAG);
   shaders[BLANK_SHADER] = new Shader(BLANK_SHADER_TEXTURE_VERT, BLANK_SHADER_TEXTURE_FRAG);
   shaders[CUBEMAP_SHADER] = new Shader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
   shaders[TEXT_SHADER] = new Shader;
   log_info("shaders init done");
}

void Resources::init_resources(){

   load_shaders();
   load_textures();
   init_text();
   init_models();
}

void Resources::cleanup(){
   for (auto i: shaders){
      i->cleanup();
   }
   for (auto i: textures){
      i->cleanup();
   }
   for (auto i: models){
      for (int j = 0; j < i.size(); j++){
         i[j]->cleanup();
      }
   }
   
}
