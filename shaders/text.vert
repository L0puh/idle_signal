#version 330 core
layout (location = 0) in vec2 pos; 
layout (location = 1) in vec2 tex; 

out vec2 _tex_coord;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(pos.xy, 0.0, 1.0);
    _tex_coord = tex;
}
