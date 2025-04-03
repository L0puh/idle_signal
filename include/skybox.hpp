#ifndef SKYBOX_H
#define SKYBOX_H 
#include "camera.hpp"
#include "core.hpp"
#include "vertices.hpp"

class Skybox {
   int count_vertices;
   Texture *cubemap_tex;
   Shader *cubemap_shd;
   Vertex vertex;

   public:
      Skybox() {
         vertex.create_VBO(vertices::skybox, sizeof(vertices::skybox));
         vertex.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float)); 
         count_vertices = LEN(vertices::skybox);

         cubemap_tex = state.resources->textures[CUBEMAP_TEXTURE];
         cubemap_shd = state.resources->shaders[CUBEMAP_SHADER];
      };
      ~Skybox() {
         vertex.cleanup();
      }

   public:
      void draw(){
         glDepthFunc(GL_LEQUAL);
         glm::mat4 model = glm::mat4(1.0f);
         model = state.camera->get_projection() * glm::mat4(glm::mat3(state.camera->get_view()));
         cubemap_shd->use();
         cubemap_shd->set_mat4fv("_model", model);
         vertex.bind();
         cubemap_tex->use_cubemap();
         vertex.draw_VBO(GL_TRIANGLES, count_vertices);
         glDepthFunc(GL_LESS);
      };
   

};

#endif 
