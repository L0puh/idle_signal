#version 330 core

out vec4 color;
uniform vec3 _color;

void main() {
   color = vec4(_color, 1.0f);
}
