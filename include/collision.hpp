#ifndef COLLISION_HPP
#define COLLISION_HPP 

#include "core.hpp"
#include "glm/geometric.hpp"
#include "model.hpp"
#include "renderer.hpp"

#include <vector>

struct collider_t {
   glm::vec3 pos;
   glm::vec3 size;
   glm::vec3 min;
   glm::vec3 max;
   Model *model;
   Object *obj;
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
      void add_collider(Object *object, glm::vec3 min, glm::vec3 max) {
         collider_t collider = {object->pos, object->size, min, max};
         collider.model = NULL;
         collider.obj = object;
         colliders.push_back(collider);
      }
      void update_colliders(){
         collider_t coll;
         line_data_t data;

         for (int i = 0; i  < colliders.size(); i++){
            coll = colliders.at(i);
            if (coll.model != NULL){
               data = coll.model->get_line_data();
               state.renderer->draw_line(coll.max, coll.min,
                        color::red, 5.0f, state.default_shader,
                        data);
               state.renderer->draw_cube(coll.min, coll.max, color::black, state.default_shader, 
                     data);
               colliders.at(i) = {data.pos, data.size, coll.min, coll.max, coll.model};
            } else if (coll.obj != NULL){
               colliders.at(i) = {coll.obj->pos, coll.obj->size, coll.min, coll.max, coll.model, coll.obj};
            }
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
            if (!colliders.at(i).model->is_picked && check_AABB(*collider, colliders.at(i))) {
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

      bool point_inside_circle(glm::vec2 p, glm::vec2 pc, float r){
         glm::vec2 dist = p - pc;
         float len = glm::length(dist);
         return len <= r;
      }
      bool point_on_segment(glm::vec2 from, glm::vec2 to, glm::vec2 p){
         float d1 = glm::distance(p, from);
         float d2 = glm::distance(p, to);
         float len = glm::distance(from, to);
         return d1+d2 >= len - 0.1f && d1+d2 <= len + 0.1f;
      }
      bool line_circle(glm::vec2 from, glm::vec2 to, glm::vec2 p, float r){
         if (point_inside_circle(from, p, r) || point_inside_circle(to, p, r)) 
            return true;

         float len = glm::distance(from, to);
         float dot = ( ((p.x-from.x)*(to.x-from.x)) + ((p.y-from.y)*(to.y-from.y)) ) / (len*len);
         glm::vec2 closest;
         closest.x = from.x + (dot * (to.x-from.x));
         closest.y = from.y + (dot * (to.y-from.y));
         if (!point_on_segment(from, to, closest)) return false;

         // draw closest point
         state.renderer->draw_circle(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f,
                  0.0f), 0.1f, color::red, state.default_shader, {
               glm::vec3(closest.x, state.ground_level, closest.y),
               glm::vec3(1.0f)});
         
         glm::vec2 dist = closest - p;
         return glm::length(dist) <= r;

      }

};

#endif
