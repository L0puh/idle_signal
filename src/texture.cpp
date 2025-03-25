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

void Texture::load_texture(){
   unsigned char *data;
   int width, height, channels;   
   
   glGenTextures(1, &id);
   use();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   if (is_flip)
      stbi_set_flip_vertically_on_load(true);
   data = stbi_load(path.c_str(), &width, &height, &channels, 0);
   

   
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

