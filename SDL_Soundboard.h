#ifndef SDL_SOUNDBOARD_H
#define SDL_SOUNDBOARD_H

#include <iostream>
#include <map>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#define SDL_FLAGS SDL_INIT_AUDIO

#define MIX_DEFAULT_FREQUENCY 44100
#define MIX_DEFAULT_FORMAT SDL_AUDIO_S16
#define MIX_DEFAULT_CHANNELS 2


class SDL_Soundboard {
	
	MIX_Mixer *mixer;
	MIX_Track *music_track;
	MIX_Track *sfx_track;
	
	std::map<std::string, MIX_Audio*> audios;
	
	public:
		SDL_Soundboard();
		~SDL_Soundboard();
		
		void assign_audios(std::map<std::string, MIX_Audio*> as);
		
		void push_audio(std::string id, std::string path);
		void free_audio(std::string id);
		
		void play_music(std::string id);
		void play_sfx(std::string id);
		
	private:
		void free_tracks();
		void free_audios();
	
};

#endif
