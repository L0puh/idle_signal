#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <map>
#include <string>
#include <glm/glm.hpp>

#define MOUSE_CLICKED 0b1000
#define MOUSE_CHANGED 0b0100
#define IMGUI_FOCUSED 0b0010
#define EDITING       0b0001

#define DEFAULT_SHADER_VERT "shaders/default.vert"
#define DEFAULT_SHADER_FRAG "shaders/default.frag"

#define DEFAULT_SHADER_TEXTURE_VERT "shaders/default_texture.vert"
#define DEFAULT_SHADER_TEXTURE_FRAG "shaders/default_texture.frag"

#define WALL_SHADER_TEXTURE_VERT "shaders/wall_texture.vert"
#define WALL_SHADER_TEXTURE_FRAG "shaders/default_texture.frag"

#define TEXT_SHADER_VERT "shaders/text.vert"
#define TEXT_SHADER_FRAG "shaders/text.frag"

#define FONT_PATH "assets/fonts/freeSans.ttf"
#define EDIT_MODE  0b1000
#define PLAY_MODE  0b0100
#define PAUSE_MODE 0b0010

#define ASSIMP_FLAGS_LOAD  (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals)
const std::string TEXTURES_DIR = "assets/textures/";
const std::string MODELS_DIR  =  "assets/models/";

class World;
class Camera;
class Renderer;
class Shader;
class Collision;
class Map;

struct STATE {

   uint8_t mode;
   Collision *collision;
   Renderer *renderer;
   Camera *camera;
   Shader *default_shader;
   World *world;
   Map *map;
   
   glm::vec4 light_color;
   glm::vec4 filter_luminance_color = {0.941, 0.561, 0.024, 1.0f};
   glm::vec3 light_pos;
   float ground_level = -1.0f;
   float bg_color[4];
   float noise_intensity = 0.1f;
   float filter_threshold= 0.5f;
   float mouse_sensitivity = 0.05f;
   float deltatime  = 0.0f;
   float last_frame = 0.0f; // for deltatime
   float cooldown   = 0.4f; // seconds
   uint8_t global_state = 0.0f;
   std::map<int, bool> keys;
   std::map<int, float> keys_lastpress;
   glm::vec2 last_mouse_pos;
   bool first_mouse;        // just touched the mouse
};

extern STATE state;
#endif
