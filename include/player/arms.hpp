#ifndef ARMS_H
#define ARMS_H 

#include "core/core.hpp"
#include <vector>

#define ARMS_IDLE_FILE     "assets/models/with_animation/idle_arms.glb"
#define ARMS_PICKING_FILE  "assets/models/with_animation/picking_arms.glb"
#define ARMS_MODEL_FILE    "with_animation/arms.glb"

enum arm_actions {
   IDLE,
   PICKING,
  // punching....

   ACTIONS_CNT
};

class Arms {
   Model* model;
   Shader* shd;
   std::vector<int> animations;
   int current_action = 0;
   glm::mat4 transform = glm::mat4(1.0f);

   glm::vec3 pos_offset    = {0.0f, -5.5f, -0.6f};
   glm::vec3 size          = glm::vec3(1.0f);


   protected:
      static Arms* instance;
      Arms(){}
   public:
      static Arms* get_instance() { 
         if (instance == NULL) instance = new Arms;
         return instance;
      }

   public:
      void init();
      void update_action();
      void draw();


};

#endif /* ifndef ARMS_H */
