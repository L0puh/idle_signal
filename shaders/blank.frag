#version 330 core

out vec4 color;
in vec2 _tex_coord;
uniform sampler2D _texture;
uniform float _width;
uniform float _height;
uniform float _cell_size;


vec2 pixel_to_uv(vec2 pixelCoord) {
    return pixelCoord / vec2(_width, _height);
}

vec2 uv_to_pixel(vec2 uv) {
    return uv * vec2(_width, _height);
}

void main()
{    
   // low poly
   vec2 pixel_coord = uv_to_pixel(_tex_coord);
   vec2 cell_center = floor(pixel_coord / _cell_size) * _cell_size + (_cell_size * 0.5f);
   vec2 cell_center_uv = pixel_to_uv(cell_center);
   color = texture(_texture, cell_center_uv);
}
