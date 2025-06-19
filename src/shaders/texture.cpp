#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "core.hpp"
#include <stb_image.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


void Texture::load_font(){
   FT_Face face;
   FT_Library ft;
   
   if (FT_Init_FreeType(&ft)) 
      error_and_exit("error in init freetype library");

   if (FT_New_Face(ft, FONT_PATH, 0, &face)) {
      error_and_exit("error in font loading");
      return;
   } else {
      FT_Set_Pixel_Sizes(face, 0, 48);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      for (unsigned char c = 0; c < 128; c++){
         if (FT_Load_Char(face, c, FT_LOAD_RENDER)) 
            error_and_exit("failed to load glyph");

         uint id;
         glGenTextures(1, &id);
         use(id);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         character_t ch = { id, 
                           glm::ivec2(face->glyph->bitmap.width,
                                      face->glyph->bitmap.rows),
                           glm::ivec2(face->glyph->bitmap_left, 
                                      face->glyph->bitmap_top),
                           static_cast<uint>(face->glyph->advance.x) };
         characters[c] = ch;
         unuse();
      }
   }
   FT_Done_Face(face);
   FT_Done_FreeType(ft);

   log_info("font is loaded");
}

void Texture::load_cubemap(std::vector<std::string> faces){
   unsigned char *data;
   int width, height, channels;   
   
   glGenTextures(1, &id);
   use_cubemap();

   std::string DIR = "assets/textures/skybox/blink/";
   for (uint i = 0; i < faces.size(); i++){
      char new_src[DIR.length() + faces[i].length()];
      sprintf(new_src, "%s%s", DIR.c_str(), faces[i].c_str());
      path = new_src;
      char info[64];
      sprintf(info, "loading texture: %s", path.c_str());
      log_info(info);
      stbi_set_flip_vertically_on_load(false);
      data = stbi_load(path.c_str(), &width, &height, &channels, 0);
      if (data){
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
      }
      else {
         char info[64];
         sprintf(info, "error in loading texture[cubemap]: %s\n", path.c_str());
         error_and_exit(info);
         return;
      }
   }
   
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::load_texture(){
   unsigned char *data;
   int width, height, channels;   
   
   glGenTextures(1, &id);
   use();

   if (is_flip)
      stbi_set_flip_vertically_on_load(true);
   else 
      stbi_set_flip_vertically_on_load(false);
   data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  
   if (is_hand){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   
   if (!data){
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
   unuse();
   return;
}

