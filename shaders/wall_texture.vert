#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;

out vec3 _normal;
out vec3 _pos;
out vec2 _tex_coord;

void main()
{
   gl_Position = _projection * _view * _model * vec4(pos.x, pos.y, pos.z, 1.0f);
   _pos = vec3(_model * vec4(pos, 1.0f));
   _tex_coord = tex_coord;
   _normal = normal;
}

