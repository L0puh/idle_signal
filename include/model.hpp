#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "core.hpp"
#include "state.hpp"

class Mesh;
class Model;

struct collider_t;

class Mesh {
   public:
      Vertex vertex;
      Shader *shd;
      std::vector<data_t> vertices;
      std::vector<uint> indices;
      std::vector<Texture> textures;

   public:
      Mesh(std::vector<data_t> verts,
           std::vector<uint> ind,
           std::vector<Texture> text):
           vertices(verts), indices(ind), textures(text)
      {
         vertex.create_VAO();
         vertex.setup_mesh(this);
      };

      ~Mesh() {};
   public:
      void draw();
      void set_shader(Shader *shd) { 
         this->shd = shd;
      }

   private:

};


class Model {
   private:
      glm::vec4 color;
      bool with_texture = true;
   private:
      Shader *shd;
   public:
      glm::mat4 model;
      uint bt_object;
      bool is_picked = false;
      float rotation_angle;
      glm::vec3 pos, rotation, size;
      std::vector<Mesh> meshes;
      std::vector<Texture> textures_loaded;

      Model(const std::string src):
      size(glm::vec3(1.0f)), rotation(glm::vec3(1.0f)), 
      rotation_angle(0.0f), pos(0.0f){ 
         char new_src[MODELS_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", MODELS_DIR.c_str(), src.c_str());
         load_model(new_src);
      }
      ~Model(){};
   public:

      void load_json_file(const std::string& filename);
      void update(){
         model = glm::mat4(1.0f); 
         model = glm::translate(model, pos);
         model = glm::rotate(model, (float)rotation_angle, rotation);
         model = glm::scale(model, size);
      }
      void draw();
      void draw_debug(glm::vec3 pos, glm::vec3 size);
      void draw_debug();
      void is_with_texture(bool t) { with_texture = t; }
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

      line_data_t get_line_data(){
         return {pos, size, rotation_angle, rotation};
      }
      collider_t caclulate_boundaries();
      void cleanup(){
      }
   private:
      void load_model(const std::string src);
      void process_node(aiNode *node, const aiScene *scene);
      Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
      std::vector<Texture> load_texture(aiMaterial *mat, aiTextureType type, std::string name);

};

#endif
