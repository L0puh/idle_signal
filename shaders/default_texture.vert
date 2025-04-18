#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;

out vec2 _tex_coord;
out vec3 _normal;
out vec3 _pos;
out vec4 _eyepos;

void main()
{
   gl_Position = _projection * _view * _model * vec4(pos.x, pos.y, pos.z, 1.0f);
   _tex_coord = tex_coord;
   _normal = mat3(transpose(inverse(_model))) * normal;
   _pos = vec3(_model * vec4(pos, 1.0f));
   _eyepos = _view * _model * vec4(pos, 1.0f);
}

