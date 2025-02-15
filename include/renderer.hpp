#ifndef RENDERER_HPP
#define RENDERER_HPP 

#include "core.hpp"
#include "model.hpp"
#include "object.hpp"


class Renderer {
   private:
   public:
      void draw_line(glm::vec3 from, glm::vec3 to, const GLfloat *color, GLfloat thickness,
                     Shader *shd, Model* model = NULL){
         glLineWidth(thickness);
         Object line(object_e::line, from, to, shd);
         line.set_color(color);
         if (model != NULL){
            line.set_size(model->size);
            line.set_pos(model->pos);
            line.set_rotation(model->rotation_angle, model->rotation);
         }
         line.draw(GL_LINES);
         line.cleanup();
         glLineWidth(1.0f);
      }
      void draw_cube(glm::vec3 min, glm::vec3 max, const GLfloat *color,  Shader *shd, Model *model = NULL){

         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, min.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, min.y, min.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color, 3.0f, shd, model );
         draw_line(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, min.y, max.z), color, 3.0f, shd, model );
      }
};



#endif 
