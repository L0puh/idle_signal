#include "audio/audio.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <sndfile.h>


void Audio::init(){ 
   device = alcOpenDevice(nullptr);
   if (!device) 
      Log::get_logger()->error("no device found");
   context = alcCreateContext(device, nullptr);

   if (!context)
      Log::get_logger()->error("failed to create context of sound device");

   if (!alcMakeContextCurrent(context))
      Log::get_logger()->error("failed to make current context of sound device");

   const ALCchar* name = nullptr;
   if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
      name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
   
   if (!name || alcGetError(device) != AL_NO_ERROR)
      Log::get_logger()->error("failed to get name of sound device");
   
   Log::get_logger()->info("init audio manager");
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
      Log::get_logger()->error("couldn't open file {}", filename);
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
      Log::get_logger()->error("bad sample of sound {}", filename);
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
      Log::get_logger()->error("unsupported channel count {}", filename);
		sf_close(sndfile);
		return 0;
	}
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
      Log::get_logger()->error("failed reading samples {}", filename);
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
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
      Log::get_logger()->error("openal error in {}", filename);
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
