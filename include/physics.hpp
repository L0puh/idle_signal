#ifndef PHYSICS_H
#define PHYSICS_H 


#include "model.hpp"
#include "state.hpp"
#include <btBulletDynamicsCommon.h>


class Physics {
   private:

      btBroadphaseInterface* broadphase;
      btDefaultCollisionConfiguration* config;
      btCollisionDispatcher* dispatcher;
      btSequentialImpulseConstraintSolver* solver;
      btDiscreteDynamicsWorld* world;

      btVector3 gravity = btVector3(0, -9.81f, 0);

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
      void update_collisions();
      void add_model(Model& model);
      void update_camera_position();
      void set_camera_object();

   private:
      btCompoundShape* create_compound_shape(const Model& model);
      void add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size);
};


#endif 
