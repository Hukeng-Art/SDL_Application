#include "SDL_Soundboard.h"


SDL_Soundboard::SDL_Soundboard() {
	
	if (!SDL_Init(SDL_FLAGS)) { // initialize SDL for audio only
		fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
	}
	
	if (!MIX_Init()) { // initialize MIX component
		fprintf(stderr, "Error initializing SDL3_mixer: %s\n", SDL_GetError());
	}
	
	// create audiospec struct for mixer generation
	SDL_AudioSpec audiospec;
	audiospec.format = MIX_DEFAULT_FORMAT;
	audiospec.channels = MIX_DEFAULT_CHANNELS; // stereo or mono
	audiospec.freq = MIX_DEFAULT_FREQUENCY;
	
	// populate mixer
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audiospec);
	
	sfx_track = MIX_CreateTrack(mixer);
	music_track = MIX_CreateTrack(mixer);
	
}


SDL_Soundboard::~SDL_Soundboard() {
	
	free_audios();
	free_tracks();
	
	MIX_DestroyMixer(mixer);
	mixer = NULL;
	
	MIX_Quit();                  
	SDL_Quit(); 
	
}


void SDL_Soundboard::assign_audios(std::map<std::string, MIX_Audio*> as) {
	audios = as;
}

	
void SDL_Soundboard::push_audio(std::string id, std::string path) {
	
	MIX_Audio *audio = MIX_LoadAudio(mixer, path.c_str(), true);
	
	audios[id] = audio;
	
}


void SDL_Soundboard::free_audio(std::string id) {
	
	if (audios.find(id) == audios.end()) {
		return;
	}
	
	MIX_DestroyAudio(audios[id]);
	audios.erase(id);
}

	
void SDL_Soundboard::play_music(std::string id) {
	
	if (audios.find(id) == audios.end()) {
		fprintf(stderr, "Error - id \"%s\" does not match any loaded track.\n", id);
		return;
	}
	
	if (!MIX_SetTrackAudio(music_track, audios[id])) {
		fprintf(stderr, "Error setting music track: %s\n", SDL_GetError());
	}
	if (!MIX_PlayTrack(music_track, 0)) {
		fprintf(stderr, "Error playing music: %s\n", SDL_GetError());
	}
}


void SDL_Soundboard::play_sfx(std::string id) {
	
	if (audios.find(id) == audios.end()) {
		fprintf(stderr, "Error - id \"%s\" does not match any loaded track.\n", id);
		return;
	}
	
	if (!MIX_SetTrackAudio(sfx_track, audios[id])) {
		fprintf(stderr, "Error setting sfx track: %s\n", SDL_GetError());
	}
	if (!MIX_PlayTrack(sfx_track, 0)) {
		fprintf(stderr, "Error playing sfx: %s\n", SDL_GetError());
	}
}


void SDL_Soundboard::free_audios() {
	for (auto audio : audios) {
		free_audio(audio.first);
	}
}


void SDL_Soundboard::free_tracks() {
	
	MIX_DestroyTrack(music_track);
	music_track = NULL;
	
	MIX_DestroyTrack(sfx_track);
	sfx_track = NULL;
}
