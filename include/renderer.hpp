#ifndef RENDERER_HPP
#define RENDERER_HPP 


#include <math.h>
#include <vector>

#include "core.hpp"
#include "object.hpp"
#include "camera.hpp"

struct text_t{
   std::string str;
   glm::vec2 pos;
   float scale;
   const GLfloat* color;
   
};

class Renderer {
   public:
      Renderer() {
         state.renderer = this;
      } 
      
   public:
      std::vector<text_t> texts;
   public:

      void draw_rectangle(glm::vec3 min, glm::vec3 max, const GLfloat *color, Shader *shd, line_data_t model) {
         glm::vec3 v1(min.x, min.y, min.z); // bottom-left
         glm::vec3 v2(max.x, min.y, min.z); // bottom-right
         glm::vec3 v3(max.x, min.y, max.z); // top-right
         glm::vec3 v4(min.x, min.y, max.z); // top-left

         draw_line(v1, v2, color, 3.0f, shd, model); // bottom line
         draw_line(v2, v3, color, 3.0f, shd, model); // right line
         draw_line(v3, v4, color, 3.0f, shd, model); // top line
         draw_line(v4, v1, color, 3.0f, shd, model); // left line
      }
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
      void draw_cube(glm::vec3 min, glm::vec3 max, glm::vec4 color,  Shader *shd, line_data_t model){
         const float c[] = {color.x, color.y, color.z, color.w};
         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, min.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, min.y, min.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), c, 3.0f, shd, model);
         draw_line(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, min.y, max.z), c, 3.0f, shd, model);
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
      void draw_texts(){
         for (auto text: texts) draw_text(text);
         clear_texts();
      }
      void add_text(text_t text) { texts.push_back(text); }
      void clear_texts() { texts.clear(); }
      void draw_text(text_t text){
         Object *obj = state.resources->text_obj;
         obj->shd->use();
         obj->shd->set_mat4fv("_projection", state.camera->get_projection_ortho());
         obj->shd->set_vec3("_color", {text.color[0], text.color[1], text.color[2]});
         glActiveTexture(GL_TEXTURE0);
         obj->vert.bind();
         
         std::string::const_iterator c;
         for (c = text.str.begin(); c != text.str.end(); c++){
            character_t ch = obj->texture->characters[*c];
            glm::vec2 p, s;
            p.x = text.pos.x + ch.bearing.x * text.scale;
            p.y = text.pos.y - (ch.size.y - ch.bearing.y) * text.scale;

            s = glm::vec2(ch.size) * text.scale;

            float vertices[] = {
               p.x,       p.y + s.y, 0.0f, 0.0f,
               p.x,       p.y,       0.0f, 1.0f,
               p.x + s.x, p.y,       1.0f, 1.0f,
               
               p.x,       p.y + s.y, 0.0f, 0.0f,
               p.x + s.x, p.y,       1.0f, 1.0f,
               p.x + s.x, p.y + s.y, 1.0f, 0.0f
            };

            obj->texture->use(ch.id);
            obj->vert.sub_data(vertices, sizeof(vertices));
            obj->vert.draw_VBO(GL_TRIANGLES, 6);
            text.pos.x += (ch.advance >> 6) * text.scale;
         }
         obj->vert.unbind();
         // obj->texture->unuse();
         obj->shd->unuse();
      }

};



#endif 
