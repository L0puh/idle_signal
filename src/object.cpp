#include "object.hpp"
#include "camera.hpp"
#include "light.hpp" 

void Text::draw(GLenum mode){
   update();

   shd->use();
   shd->set_mat4fv("_projection", state.camera->get_projection());
   shd->set_mat4fv("_view", state.camera->get_view());
   shd->set_mat4fv("_model", model);
   
   if (with_texture) {
      texture->use();
      state.light->set_all(shd);
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


void Text::init(){ 
   if (texture != NULL) with_texture = true;

   texture->load_font();
   shd->init_shader(TEXT_SHADER_VERT, TEXT_SHADER_FRAG);
   vert.create_VBO(NULL, sizeof(float) * 4 * 6, GL_DYNAMIC_DRAW);
   vert.add_atrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);
}

void Line::init(glm::vec3 from, glm::vec3 to) {
   shd = state.resources->shaders[DEFAULT_SHADER];
   
   const float vertices[] = {
      from.x, from.y, from.z,
      to.x, to.y, to.z
   };
   vert.create_VBO(vertices, sizeof(vertices));
   vert.add_atrib(0, 3, GL_FLOAT, 3 * sizeof(float), 0);
   count_vertices = LEN(vertices);
}
void Line::draw(GLenum mode){
   update();

   shd->use();
   shd->set_mat4fv("_projection", state.camera->get_projection());
   shd->set_mat4fv("_view", state.camera->get_view());
   shd->set_mat4fv("_model", model);

   shd->set_vec3("_color", {color[0], color[1], color[2]});

   if (vert.with_EBO){
      vert.draw_EBO(mode, count_vertices);
   } else {
      vert.draw_VBO(mode, count_vertices);
   }
   shd->unuse();
}
