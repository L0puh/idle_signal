#include "physics.hpp"
#include "camera.hpp"
 
void Physics::init_world(){
   broadphase = new btDbvtBroadphase();
   config = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(config);
   solver = new btSequentialImpulseConstraintSolver();
   world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
   world->setGravity(gravity);
}

void Physics::update_collisions(){
   world->performDiscreteCollisionDetection();

   int num = dispatcher->getNumManifolds();
   for(int i = 0; i < num; i++) {
      btPersistentManifold* contract = dispatcher->getManifoldByIndexInternal(i);
      const btCollisionObject* x = static_cast<const btCollisionObject*>(contract->getBody0());
      const btCollisionObject* y = static_cast<const btCollisionObject*>(contract->getBody1());
      if(contract->getNumContacts() > 0) {
         //TODO:
      }
   }
}
void Physics::add_compound_model(btCompoundShape* shape, glm::vec3 pos, glm::vec3 size){
   btTransform transform;
   btCollisionObject* modelObject = new btCollisionObject();
   
   transform.setIdentity();
   transform.setOrigin({pos.x, pos.y, pos.z});
   shape->setLocalScaling({size.x, size.y, size.z});
  
   modelObject->setCollisionShape(shape);
   modelObject->setWorldTransform(transform);
   world->addCollisionObject(modelObject, 1, 1);

}

void Physics::add_model(Model& model) {
   btCompoundShape *shape = create_compound_shape(model);
   add_compound_model(shape, model.pos, model.size);
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
     btTransform localTransform;
     localTransform.setIdentity();
     shape->addChildShape(localTransform, mesh_shape);
   }

   return shape;
}

void Physics::update_camera_position(){
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(state.camera->pos.x, state.camera->pos.y, state.camera->pos.z));
    state.camera->camera_bt->setWorldTransform(transform);
}
void Physics::set_camera_object(){
   btSphereShape* shape = new btSphereShape(0.5f); 
   btCollisionObject* bt = new btCollisionObject();
   bt->setCollisionShape(shape);
   state.camera->camera_bt= bt;
   world->addCollisionObject(bt, 1, 1);
}
