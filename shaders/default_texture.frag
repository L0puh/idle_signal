#version 330 core

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _pos;
in vec4 _eyepos;

struct fog_t {
   vec3 color;
   float start;
   float end;
   float density;

   int equation;
};

struct pointlight_t{
   vec3 pos;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};



struct flashlight_t{
   vec3 pos;
   vec3 color;
   vec3 direction;
   vec3 view_pos;
   float cut_off;
   float outer_cut_off;
   float dist;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

uniform sampler2D _texture;
uniform vec3 _color;
uniform float _time;

uniform float _noise_intensity;
uniform float _threshold;
uniform vec3 _luminance_color;

uniform float _width;
uniform float _height;
uniform float _cell_size;

uniform flashlight_t _flashlight;

//FIXME

#define pointlights_count_max 20
uniform int pointlights_count;
uniform pointlight_t pointlights[pointlights_count_max];

uniform fog_t _fog;

out vec4 color;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 pixel_to_uv(vec2 pixelCoord) {
    return pixelCoord / vec2(_width, _height);
}

vec2 uv_to_pixel(vec2 uv) {
    return uv * vec2(_width, _height);
}

float fog_factor(fog_t fog, float coord){
   float res = 0.0f;
   if (fog.equation == 0){
      float len = fog.end - fog.start;
      res = (fog.end - coord) / len;
   } else if (fog.equation == 1){
      res = exp(-fog.density * coord);
   } else if (fog.equation == 2) {
      res = exp(-pow(fog.density * coord, 2.0f));
   }
   res = 1.0 - clamp(res, 0.0, 1.0);
   return res;
}

vec3 calc_pointlight(pointlight_t light, vec3 normal, vec3 pos, vec3 view_dir);

void main() {

   // low poly
   vec2 pixel_coord = uv_to_pixel(_tex_coord);
   vec2 cell_center = floor(pixel_coord / _cell_size) * _cell_size + (_cell_size * 0.5f);
   vec2 cell_center_uv = pixel_to_uv(cell_center);

   // light
   vec3 ambient = _flashlight.ambient * texture(_texture, cell_center_uv).rgb; 
   vec3 norm = normalize(_normal);
   vec3 light_dir = normalize(_flashlight.pos - _pos);
   float diff = max(dot(norm, light_dir), 0.0f);
   vec3 diffuse = _flashlight.diffuse * diff  * _flashlight.color.rgb;

   vec3 view_dir= normalize(_flashlight.view_pos - _pos);
   vec3 reflect_dir = reflect(-light_dir, norm);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 0.4f);
   vec3 specular = _flashlight.specular * spec * _flashlight.color;

   float theta = dot(light_dir, normalize(-_flashlight.direction));
   float epsilon = (_flashlight.cut_off - _flashlight.outer_cut_off);
   float intensity = clamp((theta-_flashlight.outer_cut_off)/epsilon, 0.0, 1.0);
   diffuse *= intensity;
   specular *= intensity;
   
   float dist = length(_flashlight.pos - _pos);
   float attenuation = 1.0f / (_flashlight.constant + _flashlight.linear * dist + _flashlight.quadratic * (dist * dist));
   ambient *= attenuation;
   diffuse *= attenuation;

   if (dist >= _flashlight.dist)
      color = vec4(ambient, 1.0f);
   else {
      vec3 result = (ambient + diffuse + specular);
      color = vec4(result, texture(_texture, cell_center_uv).a);
   }
   

   vec3 res = vec3(color);
   for(int i = 0; i < pointlights_count; i++)
      res += calc_pointlight(pointlights[i], norm, _pos, view_dir);
   color = vec4(res, texture(_texture, cell_center_uv).a);

   // grain noise
   float noise = rand(_tex_coord + vec2(_time));
   float grain = (noise - 0.5) * _noise_intensity;
   res = color.rgb + grain;
   res = clamp(res, 0.0, 1.0);
   color = vec4(res, color.a);

   // luminance
   vec3 rgb = color.rgb;
   float luminance = dot(rgb, _luminance_color);
   if (luminance >= _threshold) {
        res = mix(rgb, vec3(1.0), 0.3);  //blend white
    } else {
        res = mix(rgb, vec3(0.0), 0.1);  //blend black
    }

   float fog_coord = abs(_eyepos.z/_eyepos.w);
   color = vec4(res, color.a); 
   color = mix(color, vec4(_fog.color, 1.0f), fog_factor(_fog, fog_coord));
}


vec3 calc_pointlight(pointlight_t light, vec3 normal, vec3 pos, vec3 view_dir){
   vec3 dir = normalize(light.pos - pos);
   float diff = max(dot(normal, dir), 0.0);
   vec3 reflect_dir = reflect(-dir, normal);
   float dist = length(light.pos-pos);
   float camera_dist = length(light.pos - _flashlight.pos);
   dist *= camera_dist / 1.5f;
   float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

   //TODO: add material
   vec3 ambient = light.ambient;
   vec3 diffuse = light.diffuse * diff;
   ambient *= attenuation;
   diffuse *= attenuation;
   return (ambient + diffuse);
}
