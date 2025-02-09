#define STB_IMAGE_IMPLEMENTATION
#include "core.hpp"
#include <stb_image.h>

void Texture::load_texture(){
   unsigned char *data;
   int width, height, channels;   

   data = stbi_load(path.c_str(), &width, &height, &channels, 0);
   glGenTextures(1, &id);
   
   use();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   if (data == NULL){
      char info[64];
      sprintf(info, "error in loading texture: %s\n", path.c_str());
      error_and_exit(info);
      return;
   }

   if (path.find(".png") != -1){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                   width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
   } else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                   width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
   }
   
   glGenerateMipmap(GL_TEXTURE_2D);
   stbi_image_free(data);
   
   return;
}

