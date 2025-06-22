#ifndef MODEL_HPP
#define MODEL_HPP

#include "core/core.hpp"
#include "core/state.hpp"
#include "shaders/shader.hpp"
#include "shaders/texture.hpp"
#include "objects/vertex.hpp"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define MAX_BONE_WEIGHTS 4

class Mesh;
class Animator;
class Model;

struct collider_t;
struct bone_info_t {
   int id;
   glm::mat4 offset;
};

struct data_t {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texcoord;
   int   bone_ids[MAX_BONE_WEIGHTS];
   float weights[MAX_BONE_WEIGHTS];
};

class Mesh {
   public:

      bool is_animated = false;
      Vertex vertex;
      Shader *shd;
      std::vector<data_t> vertices;

      std::vector<uint> indices;
      std::vector<Texture> textures;


   public:
      Mesh(std::vector<data_t> verts,
           std::vector<uint> ind,
           std::vector<Texture> text, bool is_animated=false):
           vertices(verts), indices(ind), textures(text), 
           is_animated(is_animated)
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
   private:
      Animator *animator;
      Shader *shd;
   public:
      glm::mat4 model;
      uint bt_object;
      bool with_texture = true, with_animataion = false;
      bool is_picked = false;
      float rotation_angle;
      glm::vec3 pos, rotation, size;
      std::vector<Mesh> meshes;
      std::vector<Texture> textures_loaded;


      Model(const std::string src):
         size(glm::vec3(1.0f)), rotation(glm::vec3(1.0f)), 
         rotation_angle(0.0f), pos(0.0f)
      { 
         char new_src[MODELS_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", MODELS_DIR.c_str(), src.c_str());
         load_model(new_src);
      }
      ~Model(){};
   public:
      /* BONES */
      int bone_cnt = 0;
      std::map<std::string, bone_info_t> bone_infos;
      inline std::map<std::string, bone_info_t>& get_bone_infos() { return bone_infos; }
      inline int& get_bone_cnt() { return bone_cnt; }
      void extract_bones(std::vector<data_t>& vertices, aiMesh* mesh, const aiScene* scene);
      void set_vertex_bone(data_t& vert, int id, float weight);
      void to_defaul_vertex_data(data_t& vert);

   public:

      void update(){
         model = glm::mat4(1.0f); 
         model = glm::translate(model, pos);
         model = glm::rotate(model, (float)rotation_angle, rotation);
         model = glm::scale(model, size);
      }

      void load_json_file(const std::string& filename);
      void draw();
      void draw_debug(glm::vec3 pos, glm::vec3 size);
      void draw_debug();
      void set_with_texture(bool t) { with_texture = t; }
      void cleanup() {}


   public:
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
      inline Shader* get_shader() { return shd; }
      collider_t caclulate_boundaries();

   private:
      void load_model(const std::string src);
      void process_node(aiNode *node, const aiScene *scene);
      Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
      std::vector<Texture> load_texture(aiMaterial *mat, aiTextureType type, std::string name);

};

#endif
