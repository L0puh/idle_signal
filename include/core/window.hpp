#ifndef WINDOW_H
#define WINDOW_H 


#include "core.hpp"
#include <cstddef>


class Window {
   protected:

      static int width, height;
      static GLFWwindow* window;
      static Window* instance;
      Window () {}
   public:
      Window(const Window&) = delete;
      static Window* get_instance() {
         if (instance == NULL) {
            instance = new Window();
         }
         return instance;
      }

      static GLFWwindow* get_window() { return window; }
   public:
      GLFWwindow* init_window(int width, int height);
      void set_size(int width, int height) {
         this->width = width, this->height = height;
      }
      void update_deltatime();

      void shutdown();
      static glm::vec2 from_screen_to_ndc(const glm::ivec2& pos, const glm::ivec2& window_size);
      static glm::vec3 from_ndc_to_world(const glm::vec2& ndc);
      glm::vec2 get_window_size() { return {width, height}; }
      static int get_width() { return width; }
      static int get_height() { return height; }
      
      static void frame_buffer_size(GLFWwindow* w, int width, int height);
      static void debug_message_callback (GLenum, GLenum, GLuint, GLuint,
                                  GLsizei, const GLchar*, const GLvoid*);
};

#endif /* ifndef WINDOW_H */
