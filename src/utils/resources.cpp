#include "utils/resources.hpp"
#include "core/core.hpp"
#include "core/state.hpp"
#include "objects/object.hpp"
#include "objects/model.hpp"

#include <string>


void Resources::init_models_counter(std::string filename, models_type type) {
   std::string path = "assets/models/" + filename;
   int cnt = count_files(path)/2; // don't count .mtl
   for (int i = 1; i <= cnt; i++){
      std::string name = filename + "/" + std::to_string(i) + ".obj";
      models[type].push_back(new Model(name));
   }
}


void Resources::load_entities(){

   std::string path = "assets/entities";
   for (const auto & file: std::filesystem::directory_iterator(path)){
      init_entity(file.path());
   }
}

int Resources::init_entity(std::string filename){
   Entity* entity = new Entity(filename);
   entities[filename] = entity;
   return 0;
}

void Resources::init_models(){
   // static objects
   init_models_counter("rocks", ROCK);
   init_models_counter("buildings", BUILDING);
   init_models_counter("bushes", BUSH);
   init_models_counter("trees", TREE);
}

void Resources::init_text(){ 
   if (textures[TEXT_TEXTURE] == NULL || shaders[TEXT_SHADER] == NULL){
      Log::get_logger()->error("text init failed. no shader or texture found. init it first");
   }
   text_obj = new Text();
   Log::get_logger()->info("init text");
}

void Resources::load_textures(){
   textures[TERRAIN_TEXTURE] = new Texture("terrain.jpg");
   textures[CUBEMAP_TEXTURE] = new Texture(cubemap_faces);
   textures[TEXT_TEXTURE] = new Texture;
   Log::get_logger()->info("init textures");
}

void Resources::load_shaders(){
   shaders[DEFAULT_SHADER] = new Shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
   shaders[ANIMATION_SHADER] = new Shader(ANIMATION_SHADER_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shaders[TEXTURE_SHADER] = new Shader(DEFAULT_SHADER_TEXTURE_VERT, DEFAULT_SHADER_TEXTURE_FRAG);
   shaders[MAP_SHADER] = new Shader(MAP_SHADER_TEXTURE_VERT, MAP_SHADER_TEXTURE_FRAG);
   shaders[BLANK_SHADER] = new Shader(BLANK_SHADER_TEXTURE_VERT, BLANK_SHADER_TEXTURE_FRAG);
   shaders[CUBEMAP_SHADER] = new Shader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
   shaders[TEXT_SHADER] = new Shader;
   Log::get_logger()->info("init shaders");
}

void Resources::init_resources(){

   load_shaders();
   load_textures();
   init_text();
   init_models();
   load_entities();
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
