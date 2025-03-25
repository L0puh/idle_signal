#version 330 core

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _pos;

uniform sampler2D _texture;
uniform vec3 _color;
uniform vec4 _light_color;
uniform vec3 _light_pos;
uniform float _time;
uniform float _noise_intensity;
uniform float _threshold;
uniform vec3 _luminance_color;

out vec4 color;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
   vec4 ambient = 0.4f * _light_color; //FIXME:
   vec3 norm = normalize(_normal);
   vec3 light_dir = normalize(_light_pos - _pos);
   float diff = max(dot(norm, light_dir), 0.0f);
   vec4 diffuse = diff * _light_color;
   vec4 result = (ambient + diffuse);
   color = result *  texture(_texture, _tex_coord);
   color.a = 1.0f;

   float noise = rand(_tex_coord + vec2(_time));
   float grain = (noise - 0.5) * _noise_intensity;
   vec3 res = color.rgb + grain;
   res = clamp(res, 0.0, 1.0);
   color = vec4(res, color.a);

   vec3 rgb = color.rgb;
   float luminance = dot(rgb, _luminance_color);
   if (luminance >= _threshold) {
        res = mix(rgb, vec3(1.0), 0.3);  //blend white
    } else {
        res = mix(rgb, vec3(0.0), 0.1);  //blend black
    }

    color = vec4(res, color.a); 
}
