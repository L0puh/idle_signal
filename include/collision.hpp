#ifndef COLLISION_HPP
#define COLLISION_HPP 

#include "core.hpp"
#include "model.hpp"
#include "object.hpp"
#include <vector>

struct collider_t {
   glm::vec3 pos;
   glm::vec3 size;
   glm::vec3 min;
   glm::vec3 max;
   Model *model;
};

class Collision {
   private:
      std::vector<collider_t> colliders;
   public:
      void add_collider(Model *model) {
         collider_t collider;
         collider.pos = model->pos;
         collider.size = model->size;
         collider.min = collider.pos - collider.size * 0.5f;
         collider.max = collider.pos + collider.size * 0.5f;
         collider.model = model;
         colliders.push_back(collider);
      }
      void update_colliders(){
         for (auto collider = colliders.begin(); collider != colliders.end(); collider++){
            Model *model = collider->model;
            collider->pos = model->pos;
            collider->size = model->size;
            collider->min = collider->pos - collider->size * 0.5f;
            collider->max = collider->pos + collider->size * 0.5f;
         }
      }
      void update_collisions(){
         update_colliders();
         for (int i = 0; i < colliders.size(); i++){
            for (int j = 0; j < colliders.size(); j++){
               if (i != j && check_AABB(colliders.at(i), colliders.at(j))) {
                  // printf("COLLISION DETECTED: %f\n\n", state.deltatime);
               }
            }
         }
      }

      bool check_AABB(collider_t a, collider_t b){
         return (
          a.min.x <= b.max.x &&
          a.max.x >= b.min.x &&
          a.min.y <= b.max.y &&
          a.max.x >= b.min.y &&
          a.min.z <= b.max.z &&
          a.max.z >= b.min.z
        );
      }

      
};

#endif
