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

         init_fog();
         init_light();
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
         cubemap_shd->set_fog();
         cubemap_shd->set_mat4fv("_model", model);
         vertex.bind();
         cubemap_tex->use_cubemap();
         vertex.draw_VBO(GL_TRIANGLES, count_vertices);
         glDepthFunc(GL_LESS);
      };
      void init_light(){
         light_t light;
         light.color = glm::vec4(color::blue[0], color::blue[1], color::blue[2], 1.0f);
         light.cut_off = 12.5f;
         light.outer_cut_off = 17.5f;
         light.dist = 8.0f;
         light.ambient = glm::vec3(0.4f);
         light.diffuse = glm::vec3(0.5f);
         light.constant = 0.4f;
         light.linear = 0.0009f;
         light.quadratic = 0.032f;
         state.light = light;
      }
      void init_fog(){
         fog_t fog;
         fog.color = glm::vec3(color::dark_blue[0], color::dark_blue[1], color::dark_blue[2]);
         fog.density = 0.0145f;
         fog.start = 0.4f;
         fog.end = 0.1f;
         fog.equation = 1;
         state.fog = fog;
      }
   

};

#endif 
