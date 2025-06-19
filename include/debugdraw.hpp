#ifndef DEBUG_DRAW
#define DEBUG_DRAW 
#endif /* ifndef DEBUG_DRAW */

#include <LinearMath/btIDebugDraw.h>


class DebugDraw : public btIDebugDraw {
   public:
      int mode;
      DebugDraw(): mode(btIDebugDraw::DBG_DrawWireframe) {};

      //FIXME::
      // virtual void draw_line(const btVector3& from, const btVector3& to, const btVector3& color){
      //    Renderer::get_instance()->draw_line(glm::vec3(from.x(), from.y(), from.z()),
      //                              glm::vec3(to.x(), to.y(), to.z()),
      //                              color::red, 1.0f, Resources::get_instance()->shaders[DEFAULT_SHADER], {});
      // }

};
