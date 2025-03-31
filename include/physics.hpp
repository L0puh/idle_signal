#ifndef PHYSICS_H
#define PHYSICS_H 


#include "model.hpp"
#include "state.hpp"
#include <btBulletDynamicsCommon.h>
#include <vector>


class Physics {
   
   private:

      btBroadphaseInterface* broadphase;
      btDefaultCollisionConfiguration* config;
      btCollisionDispatcher* dispatcher;
      btSequentialImpulseConstraintSolver* solver;
      btDiscreteDynamicsWorld* world;

      btVector3 gravity = btVector3(0, -9.81f, 0);
      std::vector<btCollisionObject*> objects;

   public:
      Physics() {
         init_world();
      };

      ~Physics() {
         // FIXME: seg fault???
         // delete broadphase;
         // delete config;
         // delete dispatcher;
         // delete solver;
         // delete world;
      };

   public:
      void init_world();
      void clear_objects();
      void update_collisions();
      void add_object(btCollisionObject* obj) { world->addCollisionObject(obj); objects.push_back(obj); }
      void add_model(Model& model);
      btCollisionObject* get_object_from_vertices(std::vector<glm::vec3> vertices, const uint* indices, size_t cnt);
      void update_camera_position();
      void set_camera_object();
      void add_wall_collider(std::vector<glm::vec3> vertices);
      void add_heightmap_object(std::vector<float>& data, int width, int height, float min_height,
                           float max_height, float y_scale, float y_shift);

   public:
      btDiscreteDynamicsWorld* get_world() { return world; }
   private:
      btCompoundShape* create_compound_shape(const Model& model);
      void add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size);
};


#endif 
