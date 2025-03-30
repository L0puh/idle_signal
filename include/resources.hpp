#ifndef RESOURCES_H
#define RESOURCES_H 


#include "model.hpp"
#include <vector>


enum resource_type{
   SIZE_RESOURCES_TYPE,
};

class Resources {

   public:
      std::vector<Model*> models;

   public:
      Resources() {
         models.resize(resource_type::SIZE_RESOURCES_TYPE);
      };
      ~Resources() {};
   
   public:
      void init_models();
      void load_textures();
      void load_shaders();
      

};


#endif 
