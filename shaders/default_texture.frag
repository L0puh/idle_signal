#version 330 core

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _pos;

struct light_t{
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

uniform light_t _light;

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

void main() {

   // low poly
   vec2 pixel_coord = uv_to_pixel(_tex_coord);
   vec2 cell_center = floor(pixel_coord / _cell_size) * _cell_size + (_cell_size * 0.5f);
   vec2 cell_center_uv = pixel_to_uv(cell_center);

   // light
   vec3 ambient = _light.ambient *  texture(_texture, cell_center_uv).rgb; 
   vec3 norm = normalize(_normal);
   vec3 light_dir = normalize(_light.pos - _pos);
   float diff = max(dot(norm, light_dir), 0.0f);
   vec3 diffuse = _light.diffuse * diff  * _light.color.rgb;

   vec3 view_dir= normalize(_light.view_pos - _pos);
   vec3 reflect_dir = reflect(-light_dir, norm);
   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 0.4f);
   vec3 specular = _light.specular * spec * _light.color;

   float theta = dot(light_dir, normalize(-_light.direction));
   float epsilon = (_light.cut_off - _light.outer_cut_off);
   float intensity = clamp((theta-_light.outer_cut_off)/epsilon, 0.0, 1.0);
   diffuse *= intensity;
   specular *= intensity;
   
   float dist = length(_light.pos - _pos);
   float attenuation = 1.0f / (_light.constant + _light.linear * dist +
         _light.quadratic * (dist * dist));
   ambient *= attenuation;
   diffuse *= attenuation;

   if (dist >= _light.dist)
      color = vec4(ambient, 1.0f);
   else {
      vec3 result = (ambient + diffuse + specular);
      color = vec4(result, texture(_texture, cell_center_uv).a);
   }

   // grain noise
   float noise = rand(_tex_coord + vec2(_time));
   float grain = (noise - 0.5) * _noise_intensity;
   vec3 res = color.rgb + grain;
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

    color = vec4(res, color.a); 
}


