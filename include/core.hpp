#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdint>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "state.hpp"

#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]%s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}
#ifdef DEBUG_MODE
#define log_info(msg) {printf("[+] INFO [%s:%d]: %s\n", __func__, __LINE__, msg);}
#else
#define log(msg) {}
#endif

#define LEN(n) sizeof(n)/sizeof(n[0])

class Mesh;
class Vertex;
class Camera;
class Shader;

struct data_t {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texcoord;
};

namespace color {
   const GLfloat white[]  = {1.0f, 1.0f, 1.0f, 1.0f};
   const GLfloat black[]  = {0.0f, 0.0f, 0.0f, 1.0f};
   const GLfloat red[]    = {0.863, 0.078, 0.235, 1.0f};
   const GLfloat blue[]   = {0.255, 0.412, 0.882, 1.0f};
   const GLfloat green[]  = {0.596, 0.984, 0.596, 1.0f};
   const GLfloat yellow[] = {1.000, 0.843, 0.000, 1.0f};
}

class Texture {
   private:
      uint id;
      std::string type;
      std::string path;
      std::string name;

   public: 
      Texture(const std::string src): name(src){ 
         char new_src[TEXTURES_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", TEXTURES_DIR.c_str(), src.c_str());
         path = new_src;
         
         char info[64];
         sprintf(info, "loading texture: %s", path.c_str());
         log_info(info);
         load_texture(); 
      };
      ~Texture(){};
   public:
      void use()   { glBindTexture(GL_TEXTURE_2D, id); }
      void unuse() { glBindTexture(GL_TEXTURE_2D, 0); }
      void set_type(std::string name){ type = name; }
      std::string& get_src() { return path; }
      std::string& get_name() { return name; }

   private:
      void load_texture();
      // void cleanup();
};


class Vertex { 
   private:
      uint VAO, VBO, EBO;
   public:
      Vertex()  { 
         VAO = create_VAO(); 
      }
      ~Vertex() { 
         //FIXME cleanup(); 
       }
   public:
      bool with_EBO = false;
   public:
      int create_VAO();
      int create_EBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);
      int create_VBO(const void* data, size_t, GLenum type=GL_STATIC_DRAW);

      void bind()   { glBindVertexArray(VAO);}
      void unbind() { glBindVertexArray(0);  }

      void setup_mesh(Mesh *mesh);
      
      void add_atrib(uint id, GLint size, GLenum type, GLsizei stride, void* offset=0);
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      void update_data(const void *data, size_t);
      int cleanup();
};



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
      void draw(){
         for (uint i=0; i < meshes.size(); i++){
            meshes.at(i).draw();
         }
      }

      void set_shader(Shader *shd) { 
         this->shd = shd;
      }
   private:
      void load_model(const std::string src);
      void process_node(aiNode *node, const aiScene *scene);
      Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
      std::vector<Texture> load_texture(aiMaterial *mat, aiTextureType type, std::string name);

};
class Shader {
   private:
      uint id;
      std::unordered_map<std::string, uint> cached_locations;

   public:
      Shader() {};
      Shader(std::string vert_src, std::string frag_src, std::string geom_src="");
      ~Shader();

   public:
      void compile(uint *shader, std::string src, GLenum type);
      void link(uint vrt, uint frag, uint geom);
      void link(uint vrt, uint frag);
      void init_shader(std::string vert_src, 
                        std::string frag_src, 
                        std::string geom_src="");
      void cleanup();
      std::string load_src(std::string&);

      const uint get_id() { return id; }
      const int get_location(std::string name);
      
      void use()    { glUseProgram(id); }
      void unuse()  { glUseProgram(0);  }

      void set_mat4fv(std::string location, glm::mat4x4 mat) { 
         glUniformMatrix4fv(get_location(location), 1, GL_FALSE, glm::value_ptr(mat));
      }
      void set_vec3(std::string location, glm::vec3 vec){
         glUniform3f(get_location(location), vec.x, vec.y, vec.z);
      }
      void set_vec2(std::string location, glm::vec2 vec){
         glUniform2f(get_location(location), vec.x, vec.y);
      }
      void set_float(std::string location, float x){
         glUniform1f(get_location(location), x);
      }
};


class Camera {
   public:
      GLFWwindow* window;
      uint8_t flags;
      glm::mat4 view;
      glm::vec3 pos; 
      float speed, rotation_speed, rotation, zoom;
      double window_width, window_height;
      
   public:
      Camera(GLFWwindow* window, uint8_t flags): window(window),
         speed(1.0f), rotation_speed(1.2f), rotation(0.0f), 
         flags(flags), view(glm::mat4(1.0f)), pos(glm::vec3(0.0f)), 
         zoom(45.0f){}
   public:
      void update_movement();
      glm::vec2 unproject(glm::vec2 pos);
      glm::vec2 project(double x, double y);
      
      glm::vec2 get_mouse_pos();
      glm::mat4 get_projection_ortho();
      glm::mat4 get_projection();
      void update();

   public:
      void hide_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
      void show_cursor() { 
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      void set_pos(glm::vec3 pos)   { this->pos   = pos; }
      void set_speed(float speed)   { this->speed = speed; }
      void set_zoom(float zoom)     { this->zoom  = zoom; }
      glm::vec2 get_window_size()   { return glm::vec2(window_width, window_height); }
      glm::mat4 get_view()          { return view;}
      void clear_flag(uint8_t flag) { flags &= ~flag; }
      void set_flag(uint8_t flag)   { flags |= flag; }

};

namespace imgui {

   void init(GLFWwindow* window);
   void frame();
   void render();
   void update_focused();
   void main_draw();

}

void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                            GLsizei, const GLchar*, const GLvoid*);
void update_deltatime();
GLFWwindow* init_window(int width, int height);

#endif 
