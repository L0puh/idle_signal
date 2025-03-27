#ifndef SKYBOX_H
#define SKYBOX_H 
#include "camera.hpp"
#include "core.hpp"
#include "vertices.hpp"

class Skybox {
   int count_vertices;
   Texture cubemap;
   Shader cubemap_shd;
   Vertex vertex;
   std::vector<std::string> faces = {
       "right.png",
       "left.png",
       "top.png",
       "bottom.png",
       "front.png",
       "back.png"
   };

   public:
      Skybox() {
         cubemap.load_cubemap(faces);
         cubemap_shd.init_shader(SKYBOX_SHADER_VERT, SKYBOX_SHADER_FRAG);
         vertex.create_VBO(vertices::skybox, sizeof(vertices::skybox));
         vertex.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float)); 
         count_vertices = LEN(vertices::skybox);
      };
      ~Skybox() {
         vertex.cleanup();
      }

   public:
      void draw(){
         glDepthFunc(GL_LEQUAL);
         glm::mat4 model = glm::mat4(1.0f);
         model = state.camera->get_projection() * glm::mat4(glm::mat3(state.camera->get_view()));
         cubemap_shd.use();
         cubemap_shd.set_mat4fv("_model", model);
         vertex.bind();
         cubemap.use_cubemap();
         vertex.draw_VBO(GL_TRIANGLES, count_vertices);
         glDepthFunc(GL_LESS);
      };
   

};

#endif 
