#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;

out vec2 _tex_coord;

void main()
{
   gl_Position = _projection * _view * _model * vec4(pos.x, pos.y, pos.z, 1.0f);
   _tex_coord = tex_coord;
}

