#include "physics.hpp"
#include "camera.hpp"
#include <vector>

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
   for (auto obj: objects)
      world->removeCollisionObject(obj);
}

void Physics::update_collisions(){
   int num;
   btVector3 norm, norm_y;
   btManifoldPoint pt;
   btPersistentManifold* contract;
   btScalar dist;
   Camera* camera = state.camera;

   world->performDiscreteCollisionDetection();
   num = dispatcher->getNumManifolds();
   for(int i = 0; i < num; i++) {
      contract = dispatcher->getManifoldByIndexInternal(i);
      const btCollisionObject* x = static_cast<const btCollisionObject*>(contract->getBody0());
      const btCollisionObject* y = static_cast<const btCollisionObject*>(contract->getBody1());
      if (x != camera->camera_bt && y != camera->camera_bt) continue;
      if(contract->getNumContacts() > 0) {
         pt = contract->getContactPoint(0);
         dist = pt.getDistance();
         norm_y = pt.m_normalWorldOnB;
         norm = (x == camera->camera_bt) ? norm_y: -norm_y;
         if (dist < 0.0f) { 
             camera->pos += glm::vec3(norm.x(), norm.y(), norm.z()) * (-dist);
          }
      }
   }
   update_camera_position();
}

void Physics::add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size){
   btTransform transform;
   btCollisionObject* model = new btCollisionObject();
   
   transform.setIdentity();
   transform.setOrigin({pos.x, pos.y, pos.z});
   shape->setLocalScaling({size.x, size.y, size.z});
  
   model->setCollisionShape(shape);
   model->setWorldTransform(transform);
   world->addCollisionObject(model, 1, 1);

}

void Physics::add_model(Model& model) {
   btCompoundShape *shape = create_compound_shape(model);
   add_compound_model(shape, model.pos, model.size);
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

void Physics::update_position(uint id, glm::vec3 pos){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin({pos.x, pos.y, pos.z});
    objects[id]->setWorldTransform(transform);
}
void Physics::update_camera_position(){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin({state.camera->pos.x, state.camera->pos.y,
         state.camera->pos.z});
    state.camera->camera_bt->setWorldTransform(transform);
}
void Physics::set_camera_object(){
   btCapsuleShape* shape = new btCapsuleShape(0.5f, state.camera->height - 2.0f); 
   btCollisionObject* bt = new btCollisionObject();
   bt->setCollisionShape(shape);

   state.camera->camera_bt = bt;
   world->addCollisionObject(bt, 1, 1);
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

