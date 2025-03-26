#version 330 core
layout (location = 0) in vec3 pos; 

uniform mat4 _model;
out vec3 _tex_coord;


void main()
{
    vec4 _pos = _model * vec4(pos, 1.0);
    gl_Position = _pos.xyww;
    _tex_coord = pos;
}
