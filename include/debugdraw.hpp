#ifndef DEBUG_DRAW
#define DEBUG_DRAW 
#endif /* ifndef DEBUG_DRAW */

#include <LinearMath/btIDebugDraw.h>
#include "state.hpp"
#include "renderer.hpp"


class DebugDraw : public btIDebugDraw {
   public:
      int mode;
      DebugDraw(): mode(btIDebugDraw::DBG_DrawWireframe) {};

      //FIXME::
      virtual void draw_line(const btVector3& from, const btVector3& to, const btVector3& color){
         state.renderer->draw_line(glm::vec3(from.x(), from.y(), from.z()),
                                   glm::vec3(to.x(), to.y(), to.z()),
                                   color::red, 1.0f, state.default_shader, {});
      }

};
