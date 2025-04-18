#version 330 core

struct fog_t {
   vec3 color;
   float start;
   float end;
   float density;

   int equation;
};
out vec4 color;
in vec3 _tex_coord;
uniform samplerCube _texture;
uniform fog_t _fog;

float fog_factor(fog_t fog, float coord){
   float res = 0.0f;
   if (fog.equation == 0){
      float len = fog.end - fog.start;
      res = (fog.end - coord) / len;
   } else if (fog.equation == 1){
      res = exp(-fog.density * coord);
   } else if (fog.equation == 2) {
      res = exp(-pow(fog.density * coord, 2.0f));
   }
   res = 1.0 - clamp(res, 0.0, 1.0);
   return res;
}
void main()
{    
   color = texture(_texture, _tex_coord);
   color = mix(color, vec4(_fog.color, 1.0f), fog_factor(_fog, 100.0f));
}
