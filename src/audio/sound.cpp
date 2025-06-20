#include "audio/audio.hpp"

void Sound::pause_sound(sound_type type){
   buffer = buffers[type];
   alSourcei(source, AL_BUFFER, (ALint)buffer);

   if (states[type] == PLAY){
      alSourcePause(source);
      states[type] = PAUSE;
   }
}

void Sound::play_sound(sound_type type){
   buffer = buffers[type];
   alSourcei(source, AL_BUFFER, (ALint)buffer);
   if (states[type] == PAUSE){
      alSourcePlay(source);
      states[type] = PLAY;
   }
}
