#version 330 core

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _pos;

uniform sampler2D _texture;
uniform vec3 _color;
uniform vec4 _light_color;
uniform vec3 _light_pos;

out vec4 color;

void main() {
   vec4 ambient = 0.9f * _light_color; //FIXME:
   vec3 norm = normalize(_normal);
   vec3 light_dir = normalize(_light_pos - _pos);
   float diff = max(dot(norm, light_dir), 0.0f);
   vec4 diffuse = diff * _light_color;
   vec4 result = (ambient + diffuse);
   color = result *  texture(_texture, _tex_coord);
}
