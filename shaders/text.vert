#version 330 core
layout (location = 0) in vec4 data; 

out vec2 _tex_coord;
uniform mat4 _projection;

void main()
{
    gl_Position = _projection * vec4(data.xy, 0.0, 1.0);
    _tex_coord = data.zw;
}
