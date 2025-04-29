#include "light.hpp"
#include "camera.hpp"
#include <string>

void Light::init_flashlight() {
   light_t light;
   light.color = glm::vec4(color::blue[0], color::blue[1], color::blue[2], 1.0f);
   light.cut_off = 12.5f;
   light.outer_cut_off = 17.5f;
   light.dist = 8.0f;
   light.ambient = glm::vec3(0.4f);
   light.diffuse = glm::vec3(0.5f);
   light.constant = 0.4f;
   light.linear = 0.0009f;
   light.quadratic = 0.032f;
   flashlight = light;
}

void Light::set_flashlight(Shader *shd){

   shd->set_vec3("_flashlight.pos", state.camera->pos);
   shd->set_vec3("_flashlight.view_pos", state.camera->pos);
   shd->set_vec3("_flashlight.direction", state.camera->front);
   shd->set_vec3("_flashlight.color", flashlight.color);
   shd->set_float("_flashlight.cut_off", glm::cos(glm::radians(flashlight.cut_off)));
   shd->set_float("_flashlight.outer_cut_off", glm::cos(glm::radians(flashlight.outer_cut_off)));
   shd->set_float("_flashlight.dist", flashlight.dist); //vision distance
   shd->set_vec3("_flashlight.ambient", flashlight.ambient);
   shd->set_vec3("_flashlight.diffuse", flashlight.diffuse);
   shd->set_vec3("_flashlight.specular", glm::vec3(0.4f));
   shd->set_float("_flashlight.constant", flashlight.constant);
   shd->set_float("_flashlight.linear", flashlight.linear);
   shd->set_float("_flashlight.quadratic", flashlight.quadratic);
   shd->set_float("_time", state.deltatime);
   shd->set_float("_noise_intensity", state.noise_intensity);
   shd->set_float("_threshold", state.filter_threshold);
   shd->set_float("_width", state.camera->window_width);
   shd->set_float("_height", state.camera->window_height);
   shd->set_float("_cell_size", state.cell_size);
   shd->set_vec3("_luminance_color", state.filter_luminance_color);
   shd->set_fog();

}

void Light::set_light(Shader *shd){
   // shd->set_int("pointlights_count", point_light_pos.size());
   for (int i = 0; i < point_light_pos.size(); i++){
      std::string ind = "pointlights[" + std::to_string(i) + "].";
      shd->set_vec3(ind + "pos", point_light_pos[0]);
      shd->set_vec3(ind + "ambient", glm::vec3(0.05f, 0.05f, 0.05f));
      shd->set_vec3(ind + "diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
      shd->set_vec3(ind + "specular", glm::vec3(1.0f, 1.0f, 1.0f));
      shd->set_float(ind + "constant", 1.0f);
      shd->set_float(ind + "linear", 0.09f);
      shd->set_float(ind + "quadratic", 0.032f);
   }
}

void Light::add_point_light(glm::vec3 pos){
   point_light_pos.push_back(pos);
}
