#include "object.hpp"
#include "camera.hpp"

void Object::draw(GLenum mode){
   update();
  
   shd->use();
   shd->set_mat4fv("_projection", state.camera->get_projection());
   shd->set_mat4fv("_view", state.camera->get_view());
   shd->set_mat4fv("_model", model);
   if (with_texture) {
      texture->use();
   }
   else shd->set_vec3("_color", {color[0], color[1], color[2]});
   
   if (vert.with_EBO){
      vert.draw_EBO(mode, count_vertices);
   } else {
      vert.draw_VBO(mode, count_vertices);
   }
   if (with_texture)
      texture->unuse();
   shd->unuse();
}

void Object::generate_object(object_e type, glm::vec3 max, glm::vec3 min){ 
   if (texture != NULL) with_texture = true;

   switch(type){
      case wall:
         {
            const float vertices[] = {
               max.x, max.y, max.z, 1.0f, 1.0f,
               max.x, min.y, max.z, 1.0f, 0.0f,
               min.x, min.y, min.z, 0.0f, 0.0f,
               min.x, max.y, min.z, 0.0f, 1.0f
            };
            
            vert.create_VBO(vertices, sizeof(vertices));
            vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
            vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); //pos
            vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float))); //tex
            this->count_vertices = LEN(vertices);
            break;
         }
      case text:
         {
            texture->load_font();
            shd->init_shader(TEXT_SHADER_VERT, TEXT_SHADER_FRAG);
            vert.create_VBO(NULL, sizeof(float) * 4 * 6, GL_DYNAMIC_DRAW);
            vert.add_atrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);
            break;
         }
      default:
         break;
   }
}
