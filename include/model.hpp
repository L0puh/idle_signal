#ifndef MODEL_HPP
#define MODEL_HPP
#include "core.hpp"

class Model {
   private:
      glm::vec4 color;
      glm::vec3 pos, rotation, size;
      glm::mat4 model = glm::mat4(1.0f);
      float rotation_angle;
   private:
      Shader *shd;
   public:
      std::vector<Mesh> meshes;
      std::vector<Texture> textures_loaded;
      Model(const std::string src){ 
         char new_src[MODELS_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", MODELS_DIR.c_str(), src.c_str());
         load_model(new_src);
      }
      ~Model(){};
   public:
      void update(){
         model = glm::mat4(1.0f); 
         model = glm::translate(model, pos);
         model = glm::rotate(model, rotation_angle, rotation);
         model = glm::scale(model, size);
      }
      void draw(){
         update();
         shd->use();
         shd->set_mat4fv("_projection", state.camera->get_projection());
         shd->set_mat4fv("_view", state.camera->get_view());
         shd->set_mat4fv("_model", model);
         for (uint i=0; i < meshes.size(); i++){
            meshes.at(i).draw();
         }
         shd->unuse();
      }
      void set_shader(Shader *shd) { 
         this->shd = shd;
      }
      void set_size(glm::vec3 size) { 
         this->size = size; 
      }
      void set_pos(glm::vec3 pos) { 
         this->pos = pos; 
      }
      void set_color(glm::vec4 color) { 
         this->color = color; }
      void set_color(const GLfloat color[4]) { 
            this->color = {color[0], color[1], color[2], color[3]}; }
      void set_rotation(float angle, glm::vec3 pos) {
         this->rotation_angle = angle;
         this->rotation = pos;
      }

   private:
      void load_model(const std::string src);
      void process_node(aiNode *node, const aiScene *scene);
      Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
      std::vector<Texture> load_texture(aiMaterial *mat, aiTextureType type, std::string name);

};

#endif
