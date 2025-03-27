#ifndef AUDIO_H
#define AUDIO_H 
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

enum sound_state{
   PLAY,
   PAUSE
};

class Audio {
   public:
      ALCdevice* device;
      ALCcontext* context;
      
      std::vector<ALuint> sound_effects;
   public:
      Audio() {};
      ~Audio() {
         alDeleteBuffers(sound_effects.size(), sound_effects.data());
         sound_effects.clear();
      }

   public:
      void init();
      ALuint add_sound_effect(const char* src);
      bool remove_sound_effect(const ALuint& buffer);
};

class Sound {
   private:
      ALuint source, buffer;
      float pitch, gain;
      bool is_loop = true;
      glm::vec3 pos, vel;
      sound_state state = PAUSE;
   public:
      Sound():
         pitch(1.3f), gain(30.0f), pos(glm::vec3(0.0f)), vel(glm::vec3(0.0))
      {
         init();
      }
      ~Sound() {};

   public:
      void play_sound(const ALuint &buffer);
      void pause_sound(const ALuint &buffer);

   private:
      void init(){
         alGenSources(1, &source);
         alSourcef(source, AL_PITCH, pitch);
         alSourcef(source, AL_GAIN, pitch);
         alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
         alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
         alSourcei(source, AL_LOOPING, is_loop);
         alSourcei(source, AL_BUFFER, (ALint)buffer);
      }

};

#endif 
