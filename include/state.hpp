#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <map>

class Camera;

#define MOUSE_CLICKED 0b1000
#define MOUSE_CHANGED 0b0100
#define IMGUI_FOCUSED 0b0010
#define EDITING       0b0001

#define DEFAULT_SHADER_VERT "shaders/default.vert"
#define DEFAULT_SHADER_FRAG "shaders/default.frag"

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
