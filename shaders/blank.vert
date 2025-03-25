#version 330 core
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec2 tex_coord; 

uniform mat4 _model;
out vec2 _tex_coord;

void main()
{
    gl_Position = _model * vec4(pos, 1.0);
    _tex_coord = tex_coord;
}
