#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <map>
#include <string>

class Camera;

#define MOUSE_CLICKED 0b1000
#define MOUSE_CHANGED 0b0100
#define IMGUI_FOCUSED 0b0010
#define EDITING       0b0001

#define DEFAULT_SHADER_VERT "shaders/default.vert"
#define DEFAULT_SHADER_FRAG "shaders/default.frag"

#define DEFAULT_SHADER_TEXTURE_VERT "shaders/default_texture.vert"
#define DEFAULT_SHADER_TEXTURE_FRAG "shaders/default_texture.frag"

#define ASSIMP_FLAGS_LOAD  (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals)
const std::string TEXTURES_DIR = "assets/textures/";
const std::string MODELS_DIR  =  "assets/models/";


struct STATE {
   Camera *camera;
   float bg_color[4];
   float deltatime  = 0.0f;
   float last_frame = 0.0f; // for deltatime
   float cooldown   = 0.4f; // seconds
   float last_press = 0.0f; // seconds 
   uint8_t global_state = 0.0f;
   std::map<int, bool> keys;
};

extern STATE state;
#endif
