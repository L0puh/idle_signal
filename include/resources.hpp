#ifndef RESOURCES_H
#define RESOURCES_H 

#include "model.hpp"
#include "state.hpp"
#include <vector>


enum shader_type {
   DEFAULT_SHADER,
   MAP_SHADER,
   TEXTURE_SHADER,
   BLANK_SHADER,
   TEXT_SHADER,
   CUBEMAP_SHADER,

   SHADER_TYPE_SIZE,
};
enum texture_type {
   WALL_TEXTURE,
   CUBEMAP_TEXTURE,
   ROOF_TEXTURE,
   FLOOR_TEXTURE,
   TERRAIN_TEXTURE,
   TEXT_TEXTURE,

   TEXTURE_TYPE_SIZE,
};
enum resource_type{

   SIZE_RESOURCES_TYPE,
};

class Resources {

   public:
      std::vector<std::string> cubemap_faces = {
          "right.png",
          "left.png",
          "top.png",
          "bottom.png",
          "front.png",
          "back.png"
      };

      Object *text_obj;

      std::vector<Model*> models;
      std::vector<Texture*> textures;
      std::vector<Shader*> shaders;


   public:
      Resources() {
         textures.resize(texture_type::TEXTURE_TYPE_SIZE);
         shaders.resize(shader_type::SHADER_TYPE_SIZE);
         init_resources();
         state.resources = this;
      };
      ~Resources() {
         cleanup();
      };
   
   public:
      void init_resources();
      void init_text();
      void init_models();
      void load_textures();
      void load_shaders();
      void cleanup();
};


#endif 
