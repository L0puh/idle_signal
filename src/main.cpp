#include "core.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "object.hpp"

void enable_if_debug();
void shutdown(GLFWwindow*);

int main() {
   memcpy(state.bg_color, color::blue, 
         sizeof(color::blue));

   GLFWwindow *window = init_window(500, 500);
   imgui::init(window);
  
   enable_if_debug();
   Camera camera(window, 0);

   Object cube(object_e::cube);
   cube.set_pos(glm::vec3(0.0f, 0.0f, -8.0f));
   cube.set_color(color::green);
   cube.set_size(glm::vec3(1.0f, 1.0f, 1.0f));

   state.camera = &camera;
   state.camera->window_width = 3000;
   state.camera->window_height = 5000;


   Model model("assets/model.obj");
   
   printf("created %zu meshes\n",model.meshes.size());
   
   Shader shd;
   shd.init_shader(DEFAULT_SHADER_VERT, DEFAULT_SHADER_FRAG);
   while (!glfwWindowShouldClose(window)){
      imgui::frame();
      update_deltatime();
      camera.update();
      imgui::main_draw();
     
      shd.use();
      shd.set_mat4fv("_projection", state.camera->get_projection());
      shd.set_mat4fv("_view", state.camera->get_view());
   
      glm::mat4 mod = glm::mat4(1.0f);
      mod = glm::translate(mod, {0.0f, 0.0f, -13.0f});
      mod = glm::scale(mod, {0.4f, 0.4f, 0.4f});

      shd.use();
      shd.set_mat4fv("_projection", state.camera->get_projection());
      shd.set_mat4fv("_view", state.camera->get_view());
      shd.set_mat4fv("_model", mod);
      shd.set_vec3("_color", {color::black[0], color::black[1], color::black[2]});
      model.draw();
      shd.unuse();

      cube.draw();

      imgui::render();
      glfwSwapBuffers(window);
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
   
   shutdown(window);
   return 0;
}


void shutdown(GLFWwindow* window){
   glfwDestroyWindow(window);
   glfwTerminate();
   log_info("shutting down");
}

void enable_if_debug(){
#ifdef DEBUG_MODE
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(debug_message_callback, 0);
   log_info("debug mode is on");
#endif
}
