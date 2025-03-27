#ifndef WORLD_H
#define WORLD_H 

#include <btBulletDynamicsCommon.h>

#include "state.hpp"




class World {
   private:
      btDefaultCollisionConfiguration* conf; 
      btCollisionDispatcher* disp;
      btBroadphaseInterface* broadphase;
      btSequentialImpulseConstraintSolver* solver;
      btDiscreteDynamicsWorld* world;  

   public:
      World() {
         conf = new btDefaultCollisionConfiguration();
         disp = new btCollisionDispatcher(conf);
         broadphase = new btDbvtBroadphase();
         solver = new btSequentialImpulseConstraintSolver();
         world = new btDiscreteDynamicsWorld(disp, broadphase, solver, conf);
         world->setGravity(btVector3(0, -9.81f, 0));
      }
      ~World() { cleanup(); }
   public:
      void add_collision_object(btCollisionObject* obj){
         world->addCollisionObject(obj, 1, 1);
      }
      void add_collision_object(btCollisionShape* shape, glm::vec3 pos){
         btCollisionObject* obj = new btCollisionObject();
         obj->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1),
                               btVector3(pos.x, pos.y, pos.z)));
         obj->setCollisionShape(shape);
         world->addCollisionObject(obj, 1, 1);
      }
      void update() {
         world->stepSimulation(state.deltatime, 10);
         world->performDiscreteCollisionDetection();
         for (int i = 0; i < disp->getNumManifolds(); i++) {
            btPersistentManifold* contactManifold = disp->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            printf("COLLISION!: %f\n", state.deltatime);
         }

      }
      
      void cleanup() {

         //TODO: add cleanup for collision objects too
         delete world;
         delete solver;
         delete broadphase;
         delete disp;
         delete conf;
      }

};


#endif 
