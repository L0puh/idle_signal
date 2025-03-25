#ifndef HAND_H
#define HAND_H

#include "camera.hpp"
#include "core.hpp"
#include "vertices.hpp"
#include <string>
#include <vector>


class Hand{

   std::vector<Texture*> frames;
   Shader *blank;
   Vertex vert;
   int frame = 0;
   float last_change = 0.0f, framerate = 0.3f;;
   int count_vertices, count_frames = 4;

   public:
      Hand(){
         blank = new Shader(BLANK_SHADER_TEXTURE_VERT, BLANK_SHADER_TEXTURE_FRAG);
         vert.create_VBO(vertices::rectangle_with_texture, sizeof(vertices::rectangle_with_texture));
         vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
         vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); //pos
         vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float))); //tex
         count_vertices = LEN(vertices::rectangle_with_texture);
         
         for (int i = 1; i <= count_frames; i++){
            std::string name="";
            name = "frame" + std::to_string(i) + ".png";
            Texture *tex = new Texture(name, 1);
            frames.push_back(tex);
         }
      }
      ~Hand() {
         for (int i = 1; i <= count_frames; i++){
            delete frames[i];
         }
         delete blank;
      }
      void draw() {
         blank->use();
         frames[0]->use();
         if (glfwGetTime() - last_change >= framerate){
            frames[frame++]->use();
            last_change = glfwGetTime();
         }
         if (frame == count_frames) frame = 0;

         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model, glm::vec3(0.7, -0.5 + state.camera->walk_offset, 0.0f) );
         blank->set_mat4fv("_model", model);
         vert.draw_EBO(GL_TRIANGLES, count_vertices);
      
      }
};

#endif 
