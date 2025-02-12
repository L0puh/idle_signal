#include "core.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "input.hpp"

STATE state;

void frame_buffer_size(GLFWwindow* wind, int width, int height){
   glViewport(0, 0, width, height);
   if (state.camera != NULL){
      state.camera->window_width = width;
      state.camera->window_height = height;
   }
}

GLFWwindow* init_window(int width, int height){
   GLFWwindow* window;

   if (!glfwInit())
      error_and_exit("glfw init failed");

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(width, height, "window", 0, 0);
   if (window == NULL) {
     printf("[-] error[%s:%s:%d]: %s\n",
            __FILE__, __func__, 16,
            "window init failed");
     exit(-1);
   };

   glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
      error_and_exit("glad load failed");
   
   glfwSetFramebufferSizeCallback(window, frame_buffer_size);
   glfwSetMouseButtonCallback(window, input::mouse_callback);
   glfwSetKeyCallback(window, input::key_callback);
   glfwSetCursorPosCallback(window, input::cursor_callback);
   // glfwSetScrollCallback(window, input::scroll_callback); // TODO


   log_info("init window");

   glEnable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClearColor(state.bg_color[0], state.bg_color[1], state.bg_color[2], state.bg_color[3]);   
   
   return window;
}

void debug_message_callback(GLenum src, GLenum type, 
                      GLuint id,GLuint sev, GLsizei len, 
                      const GLchar* msg, const GLvoid* parm)
{
   if (type == GL_DEBUG_TYPE_ERROR){
      printf("[-] error (debug) [%d]: %s\n", type, msg);
      exit(-1);
   }
}

void update_deltatime(){
   float time;
   time = glfwGetTime();
   state.deltatime = time - state.last_frame;
   state.last_frame = time;
}
