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
   shd->unuse();
}
