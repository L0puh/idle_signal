#ifndef RENDERER_HPP
#define RENDERER_HPP 


#include <math.h>
#include <vector>

#include "core.hpp"
#include "object.hpp"

class Renderer {
   private:

   public:
      void draw_line(glm::vec3 from, glm::vec3 to, const GLfloat *color, GLfloat thickness,
                     Shader *shd, line_data_t data){
         glLineWidth(thickness);
         Object line(object_e::line, from, to, shd);
         line.set_color(color);
         line.set_size(data.size);
         line.set_pos(data.pos);
         line.set_rotation(data.rotation_angle, data.rotation);
         line.draw(GL_LINES);
         line.cleanup();
         glLineWidth(1.0f);
      }
      void draw_cube(glm::vec3 min, glm::vec3 max, const GLfloat *color,  Shader *shd, line_data_t model){
         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, min.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, min.y, min.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, min.y, max.z), color, 3.0f, shd, model);
      }

      void draw_circle(glm::vec3 center, glm::vec3 axis, float radius, const GLfloat *color, Shader *shd, line_data_t data){
         float angle;
         int segment_count;
         
         if (radius <= 0.0f)
            return;

         segment_count = 32;
         std::vector<glm::vec3> points(segment_count+1);
         for (uint32_t i = 0; i <= segment_count; i++)
         {
            angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segment_count);
            if (axis.x != 0.0f)
                points[i] = glm::vec3(center.x, cos(angle) * radius + center.y, sin(angle) * radius + center.z);
            else if (axis.y != 0.0f)
                points[i] = glm::vec3(cos(angle) * radius + center.x, center.y, sin(angle) * radius + center.z);
            else
                points[i] = glm::vec3(cos(angle) * radius + center.x, sin(angle) * radius + center.y, center.z);
         }
         for (uint32_t i = 0; i <= segment_count - 1; i++){
            draw_line(points[i], points[i + 1], color, 3.0f, shd, data);
         }
      }

      void draw_text(Object *obj, std::string text, glm::vec2 pos, float scale, const GLfloat *color){
         obj->shd->use();
         obj->shd->set_vec3("_color", glm::vec3(color[0], color[1], color[2]));
         obj->texture->use();
         obj->vert.bind();

         std::string::const_iterator c;
         for (c = text.begin(); c != text.end(); c++){
            character_t ch = obj->texture->characters.at(*c);
            glm::vec2 p, s;
            p.x = pos.x + ch.bearing.x * scale;
            p.y = pos.x - (ch.size.y - ch.bearing.x) * scale;

            s = glm::vec2(ch.size) * scale;

            float vertices[] = {
               p.x,       p.y + s.y,
               p.x,       p.y,
               p.x + s.x, p.y,
               
               p.x,       p.y + s.y,
               p.x + s.x, p.y,
               p.x + s.x, p.y + s.y
            };

            float tex[] = {
               0.0f, 0.0f,
               0.0f, 1.0f,
               1.0f, 1.0f,

               0.0f, 0.0f,
               1.0f, 1.0f,
               1.0f, 0.0f
            };
            obj->texture->use();
            obj->vert.bind_vbo();
            obj->vert.sub_data(vertices, sizeof(vertices));
            obj->vert.unbind_vbo();
            obj->vert.draw_VBO(GL_TRIANGLES, 6);
            
            pos.x += (ch.advance >> 6) * scale;
         }
         obj->vert.unbind();
         obj->texture->unuse();
      }

};



#endif 
