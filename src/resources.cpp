#include "resources.hpp"
#include "state.hpp"
#include "object.hpp"

void Resources::init_models(){
}

void Resources::init_text(){ 
   textures[TEXT_TEXTURE]->load_font();
   text_obj = new Object(object_e::text, textures[TEXT_SHADER], shaders[TEXT_SHADER]);
   log_info("init text done");
}

void Resources::load_textures(){
   textures[WALL_TEXTURE] = new Texture("wall_texture.jpg");
   textures[ROOF_TEXTURE] = new Texture("roof_texture.jpg");
   textures[FLOOR_TEXTURE] = new Texture("floor_texture.jpg");
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
   shaders[TEXT_SHADER] = new Shader(TEXT_SHADER_VERT, TEXT_SHADER_FRAG);
   log_info("shaders init done");
}

void Resources::init_resources(){
   load_shaders();
   load_textures();
   init_models();
   init_text();
}

void Resources::cleanup(){
   for (auto& i: shaders){
      delete i;
   }
   for (auto& i: textures){
      delete i;
   }
   for (auto& i: models){
      delete i;
   }
   
}
