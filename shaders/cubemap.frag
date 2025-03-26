#version 330 core

out vec4 color;
in vec3 _tex_coord;
uniform samplerCube _texture;

void main()
{    
   color = texture(_texture, _tex_coord);
}
