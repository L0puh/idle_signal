#ifndef TERRAIN_H
#define TERRAIN_H 

#include "core.hpp"
#include "resources.hpp"
#include <vector>

class Terrain {

   Vertex vert;
   Texture *texture;
   Shader *shd;
   std::vector<uint> indices;
   std::vector<std::vector<glm::vec3>> vertices;
   std::vector<std::vector<glm::vec2>> tex_coords;
   std::vector<std::vector<glm::vec3>> normals;

   std::vector<float> vbo_data;

   float MAX_HEIGHT; 
   int width, height;

   int indices_count;
   std::vector<std::vector<float>> heights;
   public:
      Terrain(float width, float height): width(width), height(height){
         texture = state.resources->textures[TERRAIN_TEXTURE];
         shd = state.resources->shaders[TEXTURE_SHADER];
         generate_heights();
         generate_vertices();
         generate_indices();
         generate_normals();
         generate_tex_coords();
         prepare_data();
         create_vertex();

         state.terrain = this;
      }
      ~Terrain(){}
      
   public:
      void draw_terrain();
      float get_height_at(float x, float z);

   private:
      void generate_vertices();
      void generate_indices();
      void generate_normals();
      void create_vertex();
      void prepare_data();
      void generate_heights();
      void generate_tex_coords();
      
};

#endif 
