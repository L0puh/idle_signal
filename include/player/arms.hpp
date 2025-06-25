#ifndef ARMS_H
#define ARMS_H 

#include "core/core.hpp"
#include <vector>

#define ARMS_IDLE_FILE     "assets/models/with_animation/idle_arms.glb"
#define ARMS_PICKING_FILE  "assets/models/with_animation/picking_arms.glb"
#define ARMS_HOLDING_FILE  "assets/models/with_animation/holding_arms.glb"
#define ARMS_MODEL_FILE    "with_animation/arms.glb"

enum arm_actions {
   IDLE,
   PICKING,
   HOLDING,
   ACTIONS_CNT
};

class Arms {
   Model* model;
   Shader* shd;
   std::vector<int> animations;
   int current_action = 0;
   glm::mat4 transform = glm::mat4(1.0f);


   public:
      glm::vec3 pos_offset    = glm::vec3(.01f, 1.8f, .01f);
      glm::vec3 size          = glm::vec3(1.0f, 1.0f, 2.205f);


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

   private:
      void update_position();


};

#endif /* ifndef ARMS_H */
