#include "camera.hpp"
#include "core.hpp"
#include "glm/trigonometric.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vert_src, std::string frag_src, std::string geom_src) {
   init_shader(vert_src, frag_src, geom_src);
}
void Shader::init_shader(std::string vert_src, std::string frag_src, std::string geom_src){
   uint vtx, frg, geom;

   compile(&vtx, vert_src, GL_VERTEX_SHADER);
   compile(&frg, frag_src, GL_FRAGMENT_SHADER);
   id = glCreateProgram();
   if (!geom_src.empty()){
      compile(&geom, geom_src, GL_GEOMETRY_SHADER);
      link(vtx, frg, geom);
      glDeleteShader(geom);
   } else {
      link(vtx, frg);
   }
   glDeleteShader(vtx);
   glDeleteShader(frg);
}

Shader::~Shader(){
   cleanup();
}

std::string Shader::load_src(std::string& src){
   std::string res;
   std::ifstream file;
   std::stringstream stream;
  
   try {
      file.open(src.c_str());
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      
      stream << file.rdbuf();
      file.close();
      res = stream.str();
   }
   catch(std::ifstream::failure error){
      error_and_exit(error.what());
   }
   return res;
}

void Shader::compile(uint *shader, std::string src, GLenum type){
   uint sh;
   const char* file_char;
   int res; char info[512];
   
   sh = glCreateShader(type);
   std::string file = load_src(src);
   file_char = file.c_str();
  
   glShaderSource(sh, 1, &file_char, NULL);
   glCompileShader(sh);

   glGetShaderiv(sh, GL_COMPILE_STATUS, &res);
   if (!res) {
      glGetShaderInfoLog(sh, 512, NULL, info);
      char* msg = (char*) malloc(512 * 2);
      sprintf(msg, "file: %s, error: %s\n", src.c_str(), info);
      error_and_exit(msg);
      free(msg);
   } 
   if (shader == NULL) error_and_exit("no shader provided");
   *shader = sh;
   
   char* msg = (char*)malloc(512 * 2);
   sprintf(msg, "file shader (%s) is loaded", src.c_str());
   log_info(msg);
   free(msg);
   return;
}

void Shader::link(uint vrt, uint frag, uint geom){
   int res; char info[512];

   glAttachShader(id, vrt);
   glAttachShader(id, geom);
   glAttachShader(id, frag);
   glLinkProgram(id);
   
   glGetProgramiv(id, GL_LINK_STATUS, &res);
   if (!res) {
      glGetProgramInfoLog(id, 512, NULL, info);
      error_and_exit(info);
   } 
   log_info("link shader");
   return;

}
void Shader::link(uint vrt, uint frag){
   int res; char info[512];

   glAttachShader(id, vrt);
   glAttachShader(id, frag);
   glLinkProgram(id);
   
   glGetProgramiv(id, GL_LINK_STATUS, &res);
   if (!res) {
      glGetProgramInfoLog(id, 512, NULL, info);
      error_and_exit(info);
   } 
   log_info("link shader");
   return;

}
void Shader::cleanup() {
   glDeleteProgram(id);
}

const int Shader::get_location(std::string name){
   int location;
   if (cached_locations[name]) 
      return cached_locations[name];
   location = glGetUniformLocation(id, name.c_str());
   if (location == -1) 
      error_and_exit(name.c_str());
   cached_locations[name] = location;
   return location;
}

void Shader::set_light(){

   set_vec3("_light.pos", state.camera->pos);
   set_vec3("_light.view_pos", state.camera->pos);
   set_vec3("_light.color", state.light_color);
   set_vec3("_light.direction", state.camera->front);
   
   set_float("_light.cut_off", glm::cos(glm::radians(12.5f)));
   set_float("_light.outer_cut_off", glm::cos(glm::radians(17.5f)));
   set_float("_light.dist", 8.0f); //vision distance
   set_vec3("_light.ambient", {0.4f, 0.4f, 0.4f});
   set_vec3("_light.diffuse", {0.5f, 0.5f, 0.5f});
   set_vec3("_light.specular", glm::vec3(0.4f));
   set_float("_light.constant", 0.4f);
   set_float("_light.linear", 0.0009f);
   set_float("_light.quadratic", 0.032f);

   set_float("_time", state.deltatime);
   set_float("_noise_intensity", state.noise_intensity);
   set_float("_threshold", state.filter_threshold);
   set_float("_width", state.camera->window_width);
   set_float("_height", state.camera->window_height);
   set_float("_cell_size", state.cell_size);
   set_vec3("_luminance_color", state.filter_luminance_color);
}
