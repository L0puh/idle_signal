#include "core.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <AL/alext.h>
#include "audio.hpp"


void Audio::init(){ 
   device = alcOpenDevice(nullptr);
   if (!device) 
      error_and_exit("no device found");
   context = alcCreateContext(device, nullptr);

   if (!context)
      error_and_exit("failed to create context");

   if (!alcMakeContextCurrent(context))
      error_and_exit("failed to make current context");

   const ALCchar* name = nullptr;
   if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
      name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
   
   if (!name || alcGetError(device) != AL_NO_ERROR)
      error_and_exit("failed to get name");
   
   log_info("init audio manager");
}


ALuint Audio::add_sound_effect(const char* filename){
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
      error_and_exit("couldn't open file");
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
      error_and_exit("bad sample");
		sf_close(sndfile);
		return 0;
	}

	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
      error_and_exit("unsupported channel count");
		sf_close(sndfile);
		return 0;
	}
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
      error_and_exit("failed reading samples");
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	sound_effects.push_back(buffer);  

	return buffer;
}

bool Audio::remove_sound_effect(const ALuint& buffer){
   auto it = sound_effects.begin();
   while (it != sound_effects.end()){
      if (*it == buffer){
         alDeleteBuffers(1, &*it);
         sound_effects.erase(it);
         return 0;
      }
      it++;
   }
   return -1;
}
void Sound::pause_sound(const ALuint& play_buffer){
   if (buffer != play_buffer){
      buffer = play_buffer;
      alSourcei(source, AL_BUFFER, (ALint)buffer);
   }
   if (state == PLAY){
      alSourcePause(source);
      state = PAUSE;
   }

}

void Sound::play_sound(const ALuint& play_buffer){
   if (buffer != play_buffer){
      buffer = play_buffer;
      alSourcei(source, AL_BUFFER, (ALint)buffer);
   }
   if (state == PAUSE){
      alSourcePlay(source);
      state = PLAY;
   }
}
