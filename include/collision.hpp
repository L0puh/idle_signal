#ifndef COLLISION_HPP
#define COLLISION_HPP 

#include "core.hpp"
#include "model.hpp"
#include "renderer.hpp"

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
         collider_t collider = model->caclulate_boundaries();
         
         collider.model = model;
         colliders.push_back(collider);
      }
      void update_colliders(){
         collider_t coll;
         line_data_t data;

         for (int i = 0; i  < colliders.size(); i++){
            coll = colliders.at(i);
            data = coll.model->get_line_data();
            state.renderer->draw_line(coll.max, coll.min,
                     color::red, 5.0f, state.default_shader,
                     data);
            state.renderer->draw_cube(coll.min, coll.max, color::black, state.default_shader, 
                  data);
            colliders.at(i) = {data.pos, data.size, coll.min, coll.max, coll.model};
         }
      }
      void update_collisions(){
         update_colliders();
         for (int i = 0; i < colliders.size(); i++){
            for (int j = i+1; j < colliders.size(); j++){
               if (check_AABB(colliders.at(i), colliders.at(j))) {
                  // TODO: resolve collision:??
                  printf("COLLISION DETECTED: %f\n", state.deltatime);
               }
            }
         }
      }

      bool AABB_collision_with(collider_t *collider){
         update_colliders();
         for (int i = 0; i < colliders.size(); i++){
            if (check_AABB(*collider, colliders.at(i))) {
               return 1;
            }
         }
         return 0;
      }

      bool check_AABB(collider_t a, collider_t b){
         a.max = glm::normalize(a.max) + a.pos;
         a.min = glm::normalize(a.min) + a.pos;
         b.max = glm::normalize(b.max) + b.pos;
         b.min = glm::normalize(b.min) + b.pos;
         return (a.max.x >= b.min.x && a.min.x <= b.max.x) &&
                (a.max.y >= b.min.y && a.min.y <= b.max.y) &&
                (a.max.z >= b.min.z && a.min.z <= b.max.z);
      }

      
};

#endif
