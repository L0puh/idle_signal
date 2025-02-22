#version 330 core
in vec2 _tex_coord;
out vec4 color;

uniform sampler2D _text;
uniform vec3 _color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(_text, _tex_coord).r);
    color = vec4(_color, 1.0) * sampled;
}
