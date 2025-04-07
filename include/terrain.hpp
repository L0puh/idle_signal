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

   int indices_count;
   std::vector<std::vector<float>> heights;
   public:
      int width, height;
      glm::vec3 center_pos = {0.0, -1.0f, 0.0f};
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
      void generate_random_coordinates(int count, std::vector<glm::vec2>* coordinates);
      bool is_within(glm::vec3 pos, float offset = 0.0f);
      bool is_within(glm::vec2 pos, float offset = 0.0f);

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
