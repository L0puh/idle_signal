#include "terrain.hpp"
#include "camera.hpp"
#include "core.hpp"
#include "physics.hpp"

#include <vector>
#include <stb_image.h>

void Terrain::generate_heightmap(std::string filename){
   int channels;
   
   unsigned char *data = stbi_load(filename.c_str(),
                                   &width, &height, &channels, 0);

   if (!data){
      char info[64];
      sprintf(info, "error in loading terrain: %s\n", filename.c_str());
      error_and_exit(info);
      return;
   }
  
   MAX_HEIGHT = 0.0f;
   for(uint i = 0; i < height; i++){
       for(uint j = 0; j < width; j++)
       {
           unsigned char* texel = data + (j + width * i) * channels;
           unsigned char y = texel[0];
           vertices.push_back( -height/2.0f + i );        
           vertices.push_back( (int)y * yscale - yshift); 

           vertices.push_back( -width/2.0f + j);        
           MAX_HEIGHT = std::max((int)y * yscale - yshift, MAX_HEIGHT);
       }
   }
   stbi_image_free(data);
   for(uint i = 0; i < height-1; i++)
       for(uint j = 0; j < width; j++)      
           for(uint k = 0; k < 2; k++)     
               indices.push_back(j + width * (i + k));
   
   NUM_STRIPS = height-1;
   NUM_VERTS_PER_STRIP = width*2;

   generate_vertices();

}

float Terrain::get_height_at(float x, float z) {
    float x_index = x + width/2.0f ;
    float z_index = z + height/2.0f ;

    if (x_index >= 0 && x_index < width - 1 && z_index >= 0 && z_index < height - 1) {
        int x_int = (int)x_index;
        int z_int = (int)z_index;
        float x_frac = x_index - x_int;
        float z_frac = z_index - z_int;

        float height_NW = vertices[(z_int * width + x_int) * 3 + 1];
        float height_NE = vertices[(z_int * width + (x_int+1)) * 3 + 1];
        float height_SW = vertices[((z_int+1) * width + x_int) * 3 + 1];
        float height_SE = vertices[((z_int+1) * width + (x_int+1)) * 3 + 1];

        float height_N = height_NW * (1 - x_frac) + height_NE * x_frac;
        float height_S = height_SW * (1 - x_frac) + height_SE * x_frac;
        float height = height_N * (1 - z_frac) + height_S * z_frac;

        return height + yshift  ;
    } else {
        return 0.0f; 
    }
}

void Terrain::add_collision(){
   state.physics->add_heightmap_object(vertices, width, height, -20.0f, MAX_HEIGHT, yscale, yshift);
}

void Terrain::generate_vertices(){
   vert.create_VBO(&vertices[0], vertices.size() * sizeof(float));
   vert.create_EBO(&indices[0], indices.size() * sizeof(uint));
   vert.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float));
   add_collision();

}

void Terrain::draw_terrain(){
   state.default_shader->use();
   
   glm::mat4 model(1.0f);
   model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0));
   model = glm::scale(model, glm::vec3(1.0));

   state.default_shader->set_mat4fv("_view", state.camera->get_view());
   state.default_shader->set_mat4fv("_projection", state.camera->get_projection());
   state.default_shader->set_mat4fv("_model", model);
   state.default_shader->set_vec3("_color", glm::vec3(color::blue[0], color::blue[1], color::blue[2]));

   vert.bind();
   for(uint strip = 0; strip < NUM_STRIPS; ++strip) {
       glDrawElements(GL_TRIANGLE_STRIP,   
                      NUM_VERTS_PER_STRIP, 
                      GL_UNSIGNED_INT,     
                      (void*)(sizeof(unsigned int)
                                * NUM_VERTS_PER_STRIP
                                * strip)); 
   }

}
