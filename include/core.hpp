#ifndef COLLCONTROL
#define COLLCONTROL 

#include <cstdio>
#include <filesystem>
#include <random>
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
#include <vector>
#include <sys/stat.h>

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
   const GLfloat dark_blue[] = {0.133, 0.125, 0.188, 1.0f};

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
      bool is_hand = false;
      bool is_flip = false;
      uint id;
      std::string type;
      std::string path;
      std::string name;

   public: 
      Texture(){}
      Texture(const std::string src, bool is_flip = false, bool is_hand = false): name(src), is_flip(is_flip), is_hand(is_hand){ 
         char new_src[TEXTURES_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", TEXTURES_DIR.c_str(), src.c_str());
         path = new_src;
         
         char info[64];
         sprintf(info, "loading texture: %s", path.c_str());
         log_info(info);
         load_texture(); 
      };
      Texture(std::vector<std::string> faces) { load_cubemap(faces); };
      ~Texture(){};
   public:
      void load_cubemap(std::vector<std::string> faces);
      void load_font();
      void use()        { glBindTexture(GL_TEXTURE_2D, id); }
      void use_cubemap() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }
      void use(uint id) { glBindTexture(GL_TEXTURE_2D, id); }
      void unuse() { glBindTexture(GL_TEXTURE_2D, 0); }
      void set_type(std::string name){ type = name; }
      std::string& get_src() { return path; }
      std::string& get_name() { return name; }
      void cleanup() {};

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
      void set_fog();

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
      void set_int(std::string location, int x){
         glUniform1i(get_location(location), x);
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
glm::vec2 from_screen_to_ndc(const glm::ivec2& pos, const glm::ivec2& window_size);
glm::vec3 from_ndc_to_world(const glm::vec2& ndc);

inline char* read_binary(const char* filename, int& size){
    FILE* f = fopen(filename, "rb");

    if (!f) {
       error_and_exit("error reading binary file(opening)");
       return nullptr;
    }

    struct stat stat_buf;
    int error = stat(filename, &stat_buf);

    if (error) {
       error_and_exit("error reading binary file(stat)");
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);
    size_t bytes_read = fread(p, 1, size, f);
    if (bytes_read != size) {
       error_and_exit("error reading binary file(reading)");
        exit(0);
    }

    fclose(f);
    return p;
}


inline int count_files(std::string dir){
   struct stat sb;
   int cnt = 0;
   std::filesystem::path out;

   for (const auto& entry : std::filesystem::directory_iterator(dir)) {
      if (stat(entry.path().c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR)){
         cnt++;
      }
   }
   return cnt;
}
inline int get_random_int(int from, int to){
   std::random_device device;
   std::mt19937 generator(device());
   std::uniform_int_distribution<std::mt19937::result_type> dist(from, to); 
   return dist(generator);
}
#endif 
