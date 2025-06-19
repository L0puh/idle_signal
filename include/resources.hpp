#ifndef RESOURCES_H
#define RESOURCES_H 

#include "entity.hpp"
#include "model.hpp"
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
   TERRAIN_TEXTURE,
   CUBEMAP_TEXTURE,
   TEXT_TEXTURE,

   TEXTURE_TYPE_SIZE,
};

enum models_type {
   NONE,
   TREE,
   ROCK,
   BUILDING,
   BUSH,

   MODELS_TYPE_SIZE,
};

class Resources {

   protected:
      static Resources *instance;
      Resources() { }

   public:
      static Resources *get_instance() { 
         if (instance == NULL) instance = new Resources();
         return instance; 
      }
  
   public:
      std::vector<std::string> cubemap_faces = {
          "right.png",
          "left.png",
          "top.png",
          "bottom.png",
          "front.png",
          "back.png"
      };

      Text *text_obj;

      std::vector<std::vector<Model*>> models;
      std::map<std::string, Entity*> entities;
      std::vector<Texture*> textures;
      std::vector<Shader*> shaders;
   
   public:
      void init() {
         textures.resize(texture_type::TEXTURE_TYPE_SIZE);
         shaders.resize(shader_type::SHADER_TYPE_SIZE);
         models.resize(models_type::MODELS_TYPE_SIZE);
         init_resources();
      }
      void init_resources();
      int  init_entity(std::string filename);
      void init_models_counter(std::string filename, models_type type);
      void init_text();
      void init_models();
      void load_entities();
      void load_textures();
      void load_shaders();
      void cleanup();
};


#endif 
