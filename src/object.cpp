#include "object.hpp"
#include "camera.hpp"
#include "vertices.hpp"
#include <vector>

void Object::draw(GLenum mode){
   update();

   shd->use();
   shd->set_mat4fv("_projection", state.camera->get_projection());
   shd->set_mat4fv("_view", state.camera->get_view());
   shd->set_mat4fv("_model", model);
   
   if (with_texture) {
      texture->use();
      shd->set_light();
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

std::vector<glm::vec3> calc_normals(std::vector<glm::vec3> vertices, const uint *indices, size_t indices_count)
{
    //FIXME  
    std::vector<glm::vec3> normals;
    for (int i=0; i < indices_count; i += 3){
       int vA = indices[i];
       int vB = indices[i+1];
       int vC = indices[i+2];

       glm::vec3 edgeAB = glm::vec3(vertices[vB] - vertices[vA]);
       glm::vec3 edgeAC = glm::vec3(vertices[vC] - vertices[vA]);
       glm::vec3 normal = glm::normalize(glm::cross(edgeAB, edgeAC));
       normals.push_back(normal);

    }
    return normals;
}


void Object::generate_object(object_e type, glm::vec3 max, glm::vec3 min){ 
   if (texture != NULL) with_texture = true;

   switch(type){
      case roof:
         {
            const float vertices[] = {
               min.x, min.y, min.z, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
               max.x, min.y, min.z, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
               max.x, min.y, max.z, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
               min.x, min.y, max.z, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
            };


            vert.create_VBO(vertices, sizeof(vertices));
            vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
            vert.add_atrib(0, 3, GL_FLOAT, 8 * sizeof(float)); //pos
            vert.add_atrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float))); //norm
            vert.add_atrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float))); //tex
            this->count_vertices = LEN(vertices);

            break;
         }
      case wall:
         {
            const float vertices[] = {
               // FIXME: add calculated normals!!!
               max.x, max.y, max.z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 
               max.x, min.y, max.z, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,   
               min.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,   
               min.x, max.y, min.z, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,   

            };

            vert.create_VBO(vertices, sizeof(vertices));
            vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
                                                                                       
            vert.add_atrib(0, 3, GL_FLOAT, 8 * sizeof(float)); //pos
            vert.add_atrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float))); //norm
            vert.add_atrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float))); //tex
            this->count_vertices = LEN(vertices);
            break;
         }
      case text:
         {
            vert.create_VBO(NULL, sizeof(float) * 4 * 6, GL_DYNAMIC_DRAW);
            vert.add_atrib(0, 4, GL_FLOAT, 4 * sizeof(float), 0);
            break;
         }
      case tiles:
         {
            const float vertices[] = {
               min.x, min.y, min.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               max.x, min.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
               max.x, min.y, max.z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
               min.x, min.y, max.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
            };
            
            vert.create_VBO(vertices, sizeof(vertices));
            vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));

            vert.add_atrib(0, 3, GL_FLOAT, 8 * sizeof(float)); //pos
            vert.add_atrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float))); //norm
            vert.add_atrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float))); //tex

            this->count_vertices = LEN(vertices);
            break;
         }
      default:
         break;
   }
}
