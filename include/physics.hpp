#ifndef PHYSICS_H
#define PHYSICS_H 


#include "model.hpp"
#include "state.hpp"
#include <btBulletDynamicsCommon.h>
#include <vector>

enum collision_type {
   NOTHING  = 0, 
   DEFAULT  = 1,
   PICKABLE = 2,
   DOOR     = 3 ,
   FLOOR    = 4, // use raycasting 
   WALL     = 5,
   STATIC   = 6, 
};

struct col_output_t {
   bool door_detected;
   bool floor_detected;

   glm::vec3 displace;
};

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
         state.physics = this;
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
      bool perform_raycast_for_camera();
      void update_position(uint id, glm::vec3 pos);
      void update_size(uint id, glm::vec3 size);
      void add_object(btCollisionObject* obj) { world->addCollisionObject(obj); objects.push_back(obj); }
      uint add_model(Model& model);
      uint add_model(Model& model, collision_type type);
      btCollisionObject* get_object_from_vertices(std::vector<glm::vec3> vertices, const uint* indices, size_t cnt);
      void update_camera_position();
      void set_camera_object();
      uint add_wall_collider(std::vector<glm::vec3> vertices);
      void add_heightmap_object(std::vector<float>& data, int width, int height, float min_height,
                           float max_height, float y_scale, float y_shift);

   public:
      btDiscreteDynamicsWorld* get_world() { return world; }
   private:
      void move_objects(col_output_t output);
      btCompoundShape* create_compound_shape(const Model& model);
      btBvhTriangleMeshShape* create_triangle_shape(const Model& model);
      uint add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size, collision_type type);
      uint add_triangle_mesh(btBvhTriangleMeshShape* shape, glm::vec3 pos, glm::vec3 size, collision_type type);
};


bool raycast(btDynamicsWorld* world, const btVector3& from, const
      btVector3& to, const btCollisionObject* to_ignore, int index, btVector3& hit);

#endif 
