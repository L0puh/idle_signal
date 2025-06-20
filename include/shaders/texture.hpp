#ifndef TEXTURE_H
#define TEXTURE_H 

#include "core/core.hpp"
#include "core/state.hpp"

#include <map>

class Texture {
   public:
      std::map<char, character_t> characters;
   
   private:
      bool is_hand = false;
      bool is_flip = false;
      uint id;
      std::string type;
      std::string path;
      std::string name;

   public: 
      Texture(){}
      Texture(const std::string src, bool is_flip = false, bool is_hand = false): name(src), is_flip(is_flip), is_hand(is_hand){ 
         char new_src[TEXTURES_DIR.length() + src.length()];
         sprintf(new_src, "%s%s", TEXTURES_DIR.c_str(), src.c_str());
         path = new_src;
         
         char info[64];
         sprintf(info, "loading texture: %s", path.c_str());
         log_info(info);
         load_texture(); 
      };
      Texture(std::vector<std::string> faces) { load_cubemap(faces); };
      ~Texture(){};
   public:
      void load_cubemap(std::vector<std::string> faces);
      void load_font();
      void use()        { glBindTexture(GL_TEXTURE_2D, id); }
      void use_cubemap() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }
      void use(uint id) { glBindTexture(GL_TEXTURE_2D, id); }
      void unuse() { glBindTexture(GL_TEXTURE_2D, 0); }
      void set_type(std::string name){ type = name; }
      std::string& get_src() { return path; }
      std::string& get_name() { return name; }
      void cleanup() {};

   private:
      void load_texture();
      // void cleanup();
};
#endif /* ifndef TEXTURE_H */
