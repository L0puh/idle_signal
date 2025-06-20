#ifndef SKYBOX_H
#define SKYBOX_H 
#include "core/camera.hpp"
#include "core/core.hpp"
#include "objects/vertices.hpp"

struct fog_t {
   glm::vec3 color;
   float start;
   float end;
   float density;
   int equation;
};

class Skybox {
   int count_vertices;
   Texture *cubemap_tex;
   Shader *cubemap_shd;
   Vertex vertex;

   protected:
      static Skybox *instance;
      Skybox() { 
      }

   public:
      static Skybox *get_instance() { 
         if (instance == NULL) instance = new Skybox();
         return instance; 
      }

   public:
      fog_t fog;
      void draw(){
         glDepthFunc(GL_LEQUAL);
         glm::mat4 model = glm::mat4(1.0f);
         Camera *cam = Camera::get_instance();
         model = cam->get_projection()  * glm::mat4(glm::mat3(cam->get_view()));
         cubemap_shd->use();
         cubemap_shd->set_fog();
         cubemap_shd->set_mat4fv("_model", model);
         vertex.bind();
         cubemap_tex->use_cubemap();
         vertex.draw_VBO(GL_TRIANGLES, count_vertices);
         glDepthFunc(GL_LESS);
      };
      
      void init() {
         vertex.create_VBO(vertices::skybox, sizeof(vertices::skybox));
         vertex.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float)); 
         count_vertices = LEN(vertices::skybox);

         cubemap_tex = Resources::get_instance()->textures[CUBEMAP_TEXTURE];
         cubemap_shd = Resources::get_instance()->shaders[CUBEMAP_SHADER];

         init_fog();
      }
      void init_fog(){
         fog.color = glm::vec3(color::dark_blue[0], color::dark_blue[1], color::dark_blue[2]);
         fog.density = 0.0145f;
         fog.start = 0.4f;
         fog.end = 0.1f;
         fog.equation = 1;
      }
   

};

#endif 
