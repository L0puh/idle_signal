#ifndef TERRAIN_H
#define TERRAIN_H 

#include "core.hpp"
#include <string>

class Terrain {

   Vertex vert;
   std::vector<float> vertices;
   std::vector<uint> indices;
   unsigned int NUM_STRIPS;
   unsigned int NUM_VERTS_PER_STRIP;
   
   float yscale= 64.0f / 256.0f, yshift = 30.0f;  
   int width, height;
   public:
      Terrain(){}
      ~Terrain(){}
      
   public:
      void generate_heightmap(std::string filename);
      void draw_terrain();
      float get_height_at(float x, float z);
      void add_collision();

   private:
      void generate_vertices();
      
};

#endif 
