#ifndef COLLCONTROL
#define COLLCONTROL 

#include <assimp/matrix4x4.h>
#include <cstdio>
#include <filesystem>
#include <random>
#include <string>
#include <sys/types.h>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sys/stat.h>



#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]%s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}
#ifdef DEBUG_MODE
#define log_info(msg) {printf("[+] INFO [%s:%d]: %s\n", __func__, __LINE__, msg);}
#else
#define log_info(msg) {}
#endif

#define LEN(n) sizeof(n)/sizeof(n[0])

class Window;
class Light;
class Camera;
class Text;
class Texture;
class Shader;
class Vertex;
class Model;
class Mesh;

struct line_data_t {
   glm::vec3 pos;
   glm::vec3 size;
   float rotation_angle = 0.0f;
   glm::vec3 rotation = glm::vec3(1.0f);
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





namespace imgui {

   void init();
   void frame();
   void render();
   void update_focused();
   void main_draw();

}

glm::vec3 get_normals(const float* vertices);
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

inline glm::mat4 matrix_to_glm(const aiMatrix4x4& from){
   glm::mat4 to;
   to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
   to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
   to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
   to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
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
