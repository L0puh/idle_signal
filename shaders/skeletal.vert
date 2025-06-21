#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in ivec4 bone_ids; 
layout(location = 4) in vec4 weights;
	
	
uniform mat4 _model;
uniform mat4 _projection;
uniform mat4 _view;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final[MAX_BONES];
	
out vec2 _tex_coord;
out vec3 _normal;
out vec3 _pos;
out vec4 _eyepos;
	
void main()
{
    vec4 total = vec4(0.0f);
    vec3 total_norm = vec3(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(bone_ids[i] == -1) 
            continue;
        if(bone_ids[i] >= MAX_BONES) 
        {
            total = vec4(pos,1.0f);
            break;
        }
        vec4 local_pos = final[bone_ids[i]] * vec4(pos,1.0f);
        total += local_pos * weights[i];
        total_norm += mat3(final[bone_ids[i]]) * norm * weights[i];
    }
		
    gl_Position =  _projection * _view * (_model * total);
   _tex_coord = tex_coord;
   _normal = mat3(transpose(inverse(_model))) * normalize(total_norm);
   _pos = vec3(_model * total);
   _eyepos = _view * _model * total; 
}

