#ifndef RENDERER_HPP
#define RENDERER_HPP 

#include "core.hpp"
#include "object.hpp"


class Renderer {
   private:
   public:
      void draw_line(glm::vec3 from, glm::vec3 to, const GLfloat *color, GLfloat thickness,
                     Shader *shd){
         glLineWidth(thickness);
         Object line(object_e::line, from, to, shd);
         line.set_color(color);

         line.draw(GL_LINES);
         line.cleanup();
         glLineWidth(1.0f);
      }
};



#endif 
