#ifndef SDL_APPLICATION_H
#define SDL_APPLICATION_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <iostream>
#include <memory>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SDL_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)

// audio mixer defaults
#define MIX_DEFAULT_FREQUENCY 44100
#define MIX_DEFAULT_FORMAT SDL_AUDIO_S16
#define MIX_DEFAULT_CHANNELS 2

class SDL_Application {
	
protected:
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	
	MIX_Mixer *mixer;
	MIX_Track *sfx_track;
	MIX_Track *music_track;
	
	bool is_running;
	
	//input and related values
	const bool* key_states;
	
	float mouse_x;
	float mouse_y;
	
	// delta and related values
	uint64_t last_time;
	uint64_t current_time;
	double delta;
	
	// Default values - override in subclass
	int window_width = 800;
	int window_height = 500;
	bool window_resizable = true;
	const char* window_title = "Application";
	const char* icon = "assets/icon.png";
	
	
public:

	SDL_Application();
	~SDL_Application();
	
	void run();
	
protected:

	void events();
	void update();
	void draw();
	
private:	
	// Virtual functions - extend in child classes
	virtual void events_ext() {}
	virtual void update_ext() {}
	virtual void draw_ext() {}

};

#endif
