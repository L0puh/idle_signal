#include "core/core.hpp"
#include "core/window.hpp"
#include "core/camera.hpp"
#include "utils/input.hpp"

void Window::frame_buffer_size(GLFWwindow* wind, int width, int height){
   glViewport(0, 0, width, height);
   if (Camera::get_instance() != NULL){
      Window::get_instance()->set_size(width, height);
   }
}

GLFWwindow* Window::init_window(int width, int height){

   if (!glfwInit())
      Log::get_logger()->critical("glfw init failed");

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(width, height, "window", 0, 0);
   if (window == NULL) {
      Log::get_logger()->critical("window init failed");
      exit(-1);
   };

   glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
      Log::get_logger()->critical("glad load failed");
   
   glfwSetFramebufferSizeCallback(window, frame_buffer_size);
   glfwSetMouseButtonCallback(window, input::mouse_callback);
   glfwSetKeyCallback(window, input::key_callback);
   glfwSetCursorPosCallback(window, input::cursor_callback);
   // glfwSetScrollCallback(window, input::scroll_callback); // TODO

   // glfwWindowHint(GLFW_DEPTH_BITS, 24);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   return window;
}

void Window::debug_message_callback(GLenum src, GLenum type, 
                      GLuint id,GLuint sev, GLsizei len, 
                      const GLchar* msg, const GLvoid* parm)
{
   if (type == GL_DEBUG_TYPE_ERROR){
      Log::get_logger()->error("({}){}", type, msg);
      exit(-1);
   }
}

void Window::update_deltatime(){
   float time = glfwGetTime();
   state.deltatime = time - state.last_frame;
   state.last_frame = time;
}

glm::vec3 Window::from_ndc_to_world(const glm::vec2& ndc){
    const auto r =  glm::vec4{ndc.x, ndc.y, 0.f, 1.f};
    return glm::vec3(r);
}
glm::vec2 Window::from_screen_to_ndc(const glm::ivec2& pos, const glm::ivec2& window_size){
    // return static_cast<glm::vec2>(pos) / static_cast<glm::vec2>(window_size) * 2.f -
    //        glm::vec2{1.f};
    return static_cast<glm::vec2>(pos) / static_cast<glm::vec2>(window_size);

}
void Window::shutdown(){
   glfwDestroyWindow(window);
   glfwTerminate();
   Log::get_logger()->info("shut down");
}


