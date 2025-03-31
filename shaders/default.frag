#version 330 core

out vec4 color;
in float _height;
uniform vec3 _color;

void main() {

   float h = (_height + 16)/32.0f;
   color = mix(vec4(_color, 1.0f), vec4(h, h, h, 1.0f), 0.9f);
}
