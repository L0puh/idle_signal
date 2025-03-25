#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

class Vertex;
class Shader;
class Texture;
class Mesh;
class Model;
class Camera;
class Object;

struct line_data_t {
   glm::vec3 pos;
   glm::vec3 size;
   float rotation_angle = 0.0f;
   glm::vec3 rotation = glm::vec3(1.0f);
};

struct data_t {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texcoord;
};

namespace color {
   const GLfloat white[]  = {1.0f, 1.0f, 1.0f, 1.0f};
   const GLfloat black[]  = {0.0f, 0.0f, 0.0f, 1.0f};
   const GLfloat red[]    = {0.863, 0.078, 0.235, 1.0f};
   const GLfloat grey[]   = {0.69, 0.796, 0.871, 0.839};
   const GLfloat blue[] =  {0.455, 0.612, 0.722, 0.839};
   const GLfloat green[]  = {0.596, 0.984, 0.596, 1.0f};
   const GLfloat yellow[] = {1.000, 0.843, 0.000, 1.0f};
   const GLfloat dark_grey[] = {0.137, 0.137, 0.141, 1.0f};

}

struct character_t {
   uint id;
   glm::ivec2 size;
   glm::ivec2 bearing;
   uint advance;
};

class Texture {
   public:
      std::map<char, character_t> characters;
   
   private:
      bool is_flip = false;
      uint id;
      std::string type;
      std::string path;
      std::string name;

   public: 
      Texture(){}
      Texture(const std::string src, bool is_flip = false): name(src), is_flip(is_flip){ 
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
      void load_font();
      void use()        { glBindTexture(GL_TEXTURE_2D, id); }
      void use(uint id) { glBindTexture(GL_TEXTURE_2D, id); }
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

      void bind()       { glBindVertexArray(VAO);}
      void bind_vbo()   { glBindBuffer(GL_ARRAY_BUFFER, VBO);}
      void unbind()     { glBindVertexArray(0);  }
      void unbind_vbo()  { glBindBuffer(GL_ARRAY_BUFFER, 0);  }

      void setup_mesh(Mesh *mesh);
      
      void sub_data(const void* data, size_t size);
      void add_atrib(uint id, GLint size, GLenum type, GLsizei stride, void* offset=0);
      int draw_EBO(GLenum mode, size_t size);
      int draw_VBO(GLenum mode, size_t size);
      void update_data(const void *data, size_t);
      int cleanup();
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
      void set_light();

      void set_mat4fv(std::string location, glm::mat4x4 mat) { 
         glUniformMatrix4fv(get_location(location), 1, GL_FALSE, glm::value_ptr(mat));
      }
      void set_vec3(std::string location, glm::vec3 vec){
         glUniform3f(get_location(location), vec.x, vec.y, vec.z);
      }
      void set_vec4(std::string location, glm::vec4 vec){
         glUniform4f(get_location(location), vec.x, vec.y, vec.z, vec.w);
      }
      void set_vec2(std::string location, glm::vec2 vec){
         glUniform2f(get_location(location), vec.x, vec.y);
      }
      void set_float(std::string location, float x){
         glUniform1f(get_location(location), x);
      }
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
glm::vec3 get_normals(const float* vertices);
#endif 
