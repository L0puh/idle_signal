#include "physics/physics.hpp"
#include "core/camera.hpp"

#include <vector>

#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
 
void Physics::init_world(){
   broadphase = new btDbvtBroadphase();
   config = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(config);
   solver = new btSequentialImpulseConstraintSolver();
   world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
   world->setGravity(gravity);
}

void Physics::clear_objects(){
   // camera isn't removed, just map objects 
   log_info("clearing objects");
   for (auto obj: objects)
      world->removeCollisionObject(obj);
}

bool Physics::perform_raycast_for_camera(){
   Camera *camera = Camera::get_instance();
   glm::vec3 pos = camera->pos;
   btVector3 from(pos.x, pos.y + 100.0f, pos.z); 
   btVector3 to(pos.x, pos.y - 100.0f, pos.z); 
   btVector3 hit_point;
   
   instance->update_camera_position();
   bool hit = raycast(Physics::get_instance()->get_world(), from, to, camera->camera_bt, FLOOR, hit_point);
   if (hit) {
      float y = hit_point.y() + camera->height;
      pos.y = y > pos.y ? y: pos.y;
      Camera::get_instance()->set_pos(pos);
      return 1.0f;
   } 
   return 0.0f;
}


void Physics::move_objects(col_output_t output){

   // if (output.floor_detected){
   //    perform_raycast_for_camera();
   // }
   if (output.door_detected) {
      // ... 
   } else {
      Camera::get_instance()->set_pos(Camera::get_pos() + output.displace);
   }

   update_camera_position();
}

void Physics::update_collisions(){
   int num;
   btVector3 norm, norm_y;
   col_output_t output;
   btManifoldPoint pt;
   btPersistentManifold* contract;
   btScalar dist;
   Camera* camera = Camera::get_instance();

   bool is_detected_door = false;
   
   world->performDiscreteCollisionDetection();
   num = dispatcher->getNumManifolds();

   output.displace = glm::vec3(0.0f);
   output.door_detected = false;
   output.floor_detected = false;

   for(int i = 0; i < num; i++) {
      contract = dispatcher->getManifoldByIndexInternal(i);
      const btCollisionObject* x = static_cast<const btCollisionObject*>(contract->getBody0());
      const btCollisionObject* y = static_cast<const btCollisionObject*>(contract->getBody1());
     
      if (x != camera->camera_bt && y != camera->camera_bt) continue;

      const btCollisionObject* other;

      if (x == camera->camera_bt) other = y;
      else other = x;


      int index = other->getUserIndex();
      switch(index){
         case FLOOR:
            output.floor_detected = true;
            break;
         case DOOR:
            output.door_detected = true;
            break;
         default:
            if(contract->getNumContacts() > 0) {
               pt = contract->getContactPoint(0);
               dist = pt.getDistance();
               norm_y = pt.m_normalWorldOnB;
               norm = (x == camera->camera_bt) ? norm_y: -norm_y;
               
               if (dist < 0.0f) { 
                  output.displace += glm::vec3(norm.x(), norm.y(), norm.z()) * (-dist);
                }
            }
      }
   }
   move_objects(output);
}

uint Physics::add_triangle_mesh(btBvhTriangleMeshShape* shape, glm::vec3 pos, glm::vec3 size, collision_type type){
   btTransform transform;
   btCollisionObject* model = new btCollisionObject();
   
   transform.setIdentity();
   transform.setOrigin({pos.x, pos.y, pos.z});
   shape->setLocalScaling({size.x, size.y, size.z});
   model->setCollisionFlags(model->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT); 
   model->setCollisionShape(shape);
   model->setWorldTransform(transform);
   
   model->setUserIndex(type);
   world->addCollisionObject(model, 1, 1);

   objects.push_back(model);
   return objects.size()-1;
}

uint Physics::add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size, collision_type type){
   btTransform transform;
   btCollisionObject* model = new btCollisionObject();
   
   transform.setIdentity();
   transform.setOrigin({pos.x, pos.y, pos.z});
   shape->setLocalScaling({size.x, size.y, size.z});
   model->setCollisionFlags(model->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT); 
   model->setCollisionShape(shape);
   model->setWorldTransform(transform);
   
   model->setUserIndex(type);
   world->addCollisionObject(model, 1, 1);

   objects.push_back(model);
   return objects.size()-1;

}

uint Physics::add_model(Model& model) {
   btCompoundShape *shape = create_compound_shape(model);
   return add_compound_model(shape, model.pos, model.size, DEFAULT);
}

uint Physics::add_model(Model& model, collision_type type){
   btCompoundShape *shape = create_compound_shape(model);
   return add_compound_model(shape, model.pos, model.size, type);
}


uint Physics::add_wall_collider(std::vector<glm::vec3> vertices){
     btConvexHullShape* shape = new btConvexHullShape();
     for (const auto& vertex : vertices) {
         ((btConvexHullShape*)shape)->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
     }

     btCollisionObject *wall = new btCollisionObject;
     wall->setCollisionShape(shape);
     world->addCollisionObject(wall, 1, 1);
     objects.push_back(wall);
     return objects.size()-1;
}

btCollisionObject* Physics::get_object_from_vertices(std::vector<glm::vec3> vertices, const uint* indices, size_t cnt){

   btCollisionObject* model= new btCollisionObject();
   btTriangleMesh* tri_mesh = new btTriangleMesh();

   for (int i = 0; i < cnt; i+=3){
      unsigned int i0 = indices[i];
      unsigned int i1 = indices[i+1];
      unsigned int i2 = indices[i+2];
      
      btVector3 v0(vertices[i0].x, vertices[i0].y, vertices[i0].z);
      btVector3 v1(vertices[i1].x, vertices[i1].y, vertices[i1].z);
      btVector3 v2(vertices[i2].x, vertices[i2].y, vertices[i2].z);
      
      tri_mesh->addTriangle(v0, v1, v2);
   }
   btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(tri_mesh, 1);
   btTransform transform;
   transform.setIdentity();
   transform.setOrigin({0.0f, 0.0f, 0.0f});

   model->setCollisionShape(shape);
   model->setWorldTransform(transform);

   return model;
}
btBvhTriangleMeshShape* Physics::create_triangle_shape(const Model& model){

   btTriangleMesh* tri_mesh = new btTriangleMesh();
   for(const Mesh& mesh : model.meshes) {
     
     for(size_t i = 0; i < mesh.indices.size(); i += 3) {
         unsigned int i0 = mesh.indices[i];
         unsigned int i1 = mesh.indices[i+1];
         unsigned int i2 = mesh.indices[i+2];
         
         btVector3 v0(mesh.vertices[i0].position.x, mesh.vertices[i0].position.y, mesh.vertices[i0].position.z);
         btVector3 v1(mesh.vertices[i1].position.x, mesh.vertices[i1].position.y, mesh.vertices[i1].position.z);
         btVector3 v2(mesh.vertices[i2].position.x, mesh.vertices[i2].position.y, mesh.vertices[i2].position.z);
         
         tri_mesh->addTriangle(v0, v1, v2);
     }
   }

  btBvhTriangleMeshShape* mesh_shape = new btBvhTriangleMeshShape(tri_mesh, true);
  btTransform local_transform;
  local_transform.setIdentity();

   return mesh_shape;
}

btCompoundShape* Physics::create_compound_shape(const Model& model){
   btCompoundShape* shape = new btCompoundShape();

   for(const Mesh& mesh : model.meshes) {
     btTriangleMesh* tri_mesh = new btTriangleMesh();
     
     for(size_t i = 0; i < mesh.indices.size(); i += 3) {
         unsigned int i0 = mesh.indices[i];
         unsigned int i1 = mesh.indices[i+1];
         unsigned int i2 = mesh.indices[i+2];
         
         btVector3 v0(mesh.vertices[i0].position.x, mesh.vertices[i0].position.y, mesh.vertices[i0].position.z);
         btVector3 v1(mesh.vertices[i1].position.x, mesh.vertices[i1].position.y, mesh.vertices[i1].position.z);
         btVector3 v2(mesh.vertices[i2].position.x, mesh.vertices[i2].position.y, mesh.vertices[i2].position.z);
         
         tri_mesh->addTriangle(v0, v1, v2);
     }
     
     btBvhTriangleMeshShape* mesh_shape = new btBvhTriangleMeshShape(tri_mesh, true);
     btTransform local_transform;
     local_transform.setIdentity();
     shape->addChildShape(local_transform, mesh_shape);
   }

   return shape;
}

void Physics::update_size(uint id, glm::vec3 size){
    btCollisionShape* shape = objects[id]->getCollisionShape();
    shape->setLocalScaling({size.x, size.y, size.z});
    objects[id]->setCollisionShape(shape);

}
void Physics::update_position(uint id, glm::vec3 pos){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin({pos.x, pos.y, pos.z});
    objects[id]->setWorldTransform(transform);
}
void Physics::update_camera_position(){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin({Camera::get_instance()->pos.x, Camera::get_instance()->pos.y-Camera::get_instance()->height,
         Camera::get_instance()->pos.z});
    Camera::get_instance()->camera_bt->setWorldTransform(transform);
}
void Physics::set_camera_object(){
   btCapsuleShape* shape = new btCapsuleShape(0.4f, Camera::get_instance()->height - 2.0f); 
   btCollisionObject* bt = new btCollisionObject();
   bt->setCollisionShape(shape);

   Camera::get_instance()->camera_bt = bt;
   world->addCollisionObject(bt, DEFAULT, 1);
   update_camera_position();

}
void Physics::add_heightmap_object(std::vector<float>& data, int width, int height, float min_height,
                                 float max_height, float y_scale, float y_shift){
   
   btScalar* heightfield = new btScalar[width * height];
    
    // FIXME:
    // for(int i = 0; i < height; i++) {
    //     for(int j = 0; j < width; j++) {
    //         int index = j + width * i;
    //         heightfield[index] = data[index * 3 + 1];
    //     }
    // }

    btHeightfieldTerrainShape* shape = new btHeightfieldTerrainShape(
        width, height, heightfield, 1.0f, min_height,            
        max_height, 1, PHY_FLOAT, false                  
    );


    btCollisionObject* terrain = new btCollisionObject();
    shape->setLocalScaling(btVector3(1.0f, 1.0f, 1.0f));
    terrain->setCollisionShape(shape);
    terrain->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0.0f, 0));
    terrain->setWorldTransform(transform);

    world->addCollisionObject(terrain, 1, 1);
}

bool raycast(btDynamicsWorld* world,
                     const btVector3& from,
                     const btVector3& to,
                     const btCollisionObject* to_ignore,
                     int index,
                     btVector3& hit)
{
    class callback_raycast: public btCollisionWorld::ClosestRayResultCallback
    {
    public:
        callback_raycast(const btVector3& from, const btVector3& to, const btCollisionObject* to_ignore, int index): 
           btCollisionWorld::ClosestRayResultCallback(from, to), to_ignore_m(to_ignore), index_m(index) {}

        btScalar addSingleResult(btCollisionWorld::LocalRayResult& result, bool normal){
            if (result.m_collisionObject == to_ignore_m) return 1.0f;
            if (result.m_collisionObject->getUserIndex() == index_m)
               return btCollisionWorld::ClosestRayResultCallback::addSingleResult(result, normal);
            return 1.0f;
        }

    protected:
        const btCollisionObject* to_ignore_m;
        int index_m;
    };

    callback_raycast callback(from, to, to_ignore, index);
    world->rayTest(from, to, callback);

    if (callback.hasHit()) {
        hit = callback.m_hitPointWorld;
        return true; 
    } else {
        return false; 
    }
}

