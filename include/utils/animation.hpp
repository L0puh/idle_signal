#ifndef ANIMATION_H
#define ANIMATION_H 


#include "core/core.hpp"

#include <assimp/anim.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <sys/stat.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>

#define DIR_HAND_FRAMES       "assets/textures/frames/hand/"
#define DIR_FLASHLIGHT_FRAMES "assets/textures/frames/flashlight/"

enum animation_type {
   HAND_ANIMATION,
   FLASHLIGHT
};

struct node_data_t {
   glm::mat4 model;
   std::string name;
   int children_cnt;
   std::vector<node_data_t> children;
};

struct key_pos_t {
   glm::vec3 pos;
   float time_stamp;
};

struct key_rotation_t {
   glm::quat orient;
   float time_stamp;
};
struct key_scale_t {
   glm::vec3 scale;
   float time_stamp;
};

class Bone {
  
   private: 
      int id;
      std::vector<key_pos_t>      positions;
      std::vector<key_rotation_t> rotations;
      std::vector<key_scale_t>    scales;
      glm::mat4   model;
      std::string name;

      int n_pos, n_rots, n_scales;

   public:
      Bone(const std::string& name, int bone_id, const aiNodeAnim* channel):
         name(name), id(bone_id), model(1.0f)
      {
         unpack_data(channel);
      }
   public:
      inline int         get_id()    const { return id; }
      inline std::string get_name()  const { return name; }
      inline glm::mat4   get_model() const { return model; }
   public:
      void unpack_data(const aiNodeAnim* channel);
      void update(float time);
      int get_pos_index(float time);
      int get_scale_index(float time);
      int get_rotation_index(float time);
      float get_scale_factor(float last, float next, float time);
      glm::mat4 interpolate_pos(float time);
      glm::mat4 interpolate_scale(float time);
      glm::mat4 interpolate_rotation(float time);
};


class Skeletal_animation {
   float duration;
   int fps;
   std::vector<Bone> bones;
   node_data_t root;
   std::unordered_map<std::string, bone_info_t> bone_infos;
   std::string src;

   public:
      Skeletal_animation() = default;
      Skeletal_animation(const std::string& src, Model* model): src(src){ load_animation(src, model); }
      ~Skeletal_animation(){}

   public:
      Bone* find_bone(const std::string& name);
      inline const int get_fps() { return fps; }
      inline const float get_duration() { return duration; }
      inline const node_data_t& get_root() { return root; }
      inline const std::unordered_map<std::string, bone_info_t>& get_bone_infos() { return bone_infos; }
      inline const std::string get_source() { return src; }
   
   private:
      void load_animation(const std::string& src, Model* model);
      void read_data(node_data_t& to, const aiNode* from);
      void read_missing_bones(const aiAnimation* anim, Model& model);
};

struct playback_t {
   Skeletal_animation* animation;
   float current_time = 0.0f;
   std::vector<glm::mat4> bone_models;
};

class Animator {
   int animations_counter = 0;
   int current_animation  = -1;
   std::unordered_map<int, playback_t> animations;
   
   protected:
      static Animator* instance;
      Animator(){}
   public:
      static Animator* get_instance() {
         if (instance == NULL) instance = new Animator();
         return instance;
      }

   public:
      int  add_animation(Skeletal_animation*);
      void update_animation(int animation_id=-1);
      void clear_animations();
      void set_current_animation(int id);
      void set_transform(int id, glm::vec3 pos, glm::vec3 size);
      void calc_bone_transform(const node_data_t*, glm::mat4);	
   public:
      inline const std::vector<glm::mat4> get_bone_models(int id) { 
         if (animations.count(id) == 0) 
            Log::get_logger()->error("animation with {} id isn't added", id);
         return animations[id].bone_models;
      }
};


// FIXME:
// class Frame_animation {
//    private:

//       Shader *shd;
//       Vertex vert;
     
//       float last_change = 0.0f, framerate = 20/60.0f;;
      
//       std::vector<Texture*> frames_flashlight;
//       std::vector<Texture*> frames_hand;

//       int frame = 0;
//       int count_vertices;
//       int count_frames_hand, count_frames_flashlight;
   

//    public: 
//       void init() {
//          create_sprite_vertex();
//          load_frames();
//       }
//       void cleanup(){
//          for (int i = 0; i < count_frames_hand; i++){
//             delete frames_hand[i];
//          }
//          for (int i = 0; i < count_frames_flashlight; i++){
//             delete frames_flashlight[i];
//          }
//          delete shd;
//       }
//       void draw(animation_type type){
//          float time = glfwGetTime();
//          switch (type) {
//             case HAND_ANIMATION:
//                {
//                   if (time - last_change >= framerate){
//                      last_change = time;
//                      frames_hand[frame++]->use();
//                   } else {
//                      frames_hand[frame]->use();
//                   }
//                   if (frame == count_frames_hand) frame = 0;
//                   break;
//                }
//             case FLASHLIGHT:
//             break;
//          }
//          glm::mat4 model(1.0f); 
//          glm::vec3 p = (glm::vec3(Window::get_width() - 190.0f, Camera::get_instance()->walk_offset*2.0f + 200.0f, 0.0f));
//          model = glm::translate(model, p);
//          model = glm::scale(model, glm::vec3(450.0f));
//          shd->use();
//          shd->set_mat4fv("_model", Camera::get_instance()->get_projection_ortho() * model);
//          shd->set_float("_cell_size", state.cell_size);
//          shd->set_float("_width",  Window::get_width());
//          shd->set_float("_height", Window::get_height());
//          vert.draw_EBO(GL_TRIANGLES, count_vertices);
//       }
//    private:

//       void load_frames(){
//          std::string name;
         
//          count_frames_flashlight = count_files(DIR_FLASHLIGHT_FRAMES);
//          count_frames_hand = count_files(DIR_HAND_FRAMES);
//          for (int i = 1; i <= count_frames_hand; i++){
//             name = "frames/hand/" + std::to_string(i) + ".png";
//             Texture *tex = new Texture(name, 1, 1);
//             frames_hand.push_back(tex);
//          }

//          for (int i = 1; i <= count_frames_flashlight; i++){
//             name = "frames/flashlight/" + std::to_string(i) + ".png";
//             Texture *tex = new Texture(name, 1, 1);
//             frames_hand.push_back(tex);
//          }

//       }

//       void create_sprite_vertex(){
//          shd = Resources::get_instance()->shaders[BLANK_SHADER]; 
//          vert.create_VBO(vertices::rectangle_with_texture, sizeof(vertices::rectangle_with_texture));
//          vert.create_EBO(indices::rectangle, sizeof(indices::rectangle));
//          vert.add_atrib(0, 3, GL_FLOAT, 5 * sizeof(float)); 
//          vert.add_atrib(1, 2, GL_FLOAT, 5 * sizeof(float), (3*sizeof(float))); 
//          count_vertices = LEN(vertices::rectangle_with_texture);
//       }
// };



#endif 
