#include "SDL_Application.h"


SDL_Application::SDL_Application() { // Constructor
	
	if (!SDL_Init(SDL_FLAGS)) { // Initialize SDL module
		throw std::runtime_error("Error initiating SDL.\n");
	}
	
	
	if (!TTF_Init()) { // initialize TTF module
		throw std::runtime_error("Error initiating TTF.\n");
	}
	
	
	if (!MIX_Init()) { // initialize Mixer module (audio)
		throw std::runtime_error("Error initiating MIX.\n");
	}
	
	// create window pointer
	window = SDL_CreateWindow(window_title, window_width, window_height, 0);
	
	if (!window) { // check if window pointer is populated
		throw std::runtime_error("Error creating window.\n");
	}
	
	// set window to resizable (true by default in this implementation)
	SDL_SetWindowResizable(window, window_resizable);
	
	// SDL_CreateRenderer() returns pointer to renderer for window
	renderer = SDL_CreateRenderer(window, NULL);
	
	// check if pointer is populated (not null) (alt: if (renderer == NULL))
	if (!renderer) {
		throw std::runtime_error("Error creating Renderer.\n");
	}
	
	
	// create audiospec struct to initialize mixer
	SDL_AudioSpec audiospec;
	audiospec.format = MIX_DEFAULT_FORMAT;
	audiospec.channels = MIX_DEFAULT_CHANNELS; // stereo or mono
	audiospec.freq = MIX_DEFAULT_FREQUENCY;
	
	// populate mixer and default tracks
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audiospec);
	sfx_track = MIX_CreateTrack(mixer);
	music_track = MIX_CreateTrack(mixer);
	
	
	// set up window icon - destroyed after use to free up memory
	SDL_Surface *icon_surf = IMG_Load(icon);
	if (!icon_surf) {
		throw std::runtime_error("Error loading icon surface.\n");
	}
	if (!SDL_SetWindowIcon(window, icon_surf)) {
		SDL_DestroySurface(icon_surf); // free up storage even in case of failure
		throw std::runtime_error("Error setting window icon.\n");
	}
	SDL_DestroySurface(icon_surf); // free storage
	
	key_states = SDL_GetKeyboardState(NULL); // initialize keyboard state variable
	
	is_running = true;
		
}


SDL_Application::~SDL_Application() { // DESTRUCTOR - CLEAN UP MEMORY
	
	
	if (renderer) { // if pointer to renderer is available
		SDL_DestroyRenderer(renderer); // free memory at renderer pointer
		renderer = NULL;               // set dangling pointer to null
	}
	
	if (window) { // if pointer to window is available
		SDL_DestroyWindow(window); // free memory at window pointer
		window = NULL;             // set dangling pointer to null
	}
	
	if (mixer) { // if pointer to mixer is available
		MIX_DestroyMixer(mixer); // free memory at mixer pointer
		mixer = NULL;            // set dangling pointer to null
	}
	
	MIX_Quit();         // take mixer module offline
	TTF_Quit();         // take ttf module offline
	SDL_Quit();         // take module offline after unallocating memory
	
}


void SDL_Application::run() {
	
	last_time = SDL_GetTicksNS();
	
	while(is_running) {
		events();
		update();
		draw();
	}	
}


void SDL_Application::events() {
	
	while (SDL_PollEvent(&event)) { // poll until all events are handled
	
		// check struct type of event
		// adapt action based on type
		switch (event.type) { 
			
			case SDL_EVENT_QUIT: // close game
				is_running = false;
				break;
			
			default:
				break;
		}
		
	}
	
	SDL_GetMouseState(&mouse_x, &mouse_y);
	
	events_ext();
	
}


void SDL_Application::update() {
	
	SDL_PumpEvents();
	
	// update delta variable
	current_time = SDL_GetTicksNS();
	delta = (double)(current_time - last_time) / (double)SDL_NS_PER_SECOND; // calculate duration of previous refresh cycle, set delta
	last_time = current_time;
	
	
	update_ext();
	
}


void SDL_Application::draw() {
	SDL_RenderClear(renderer); // clear renderer buffer
	
	draw_ext(); // extended draw function for actual application
	
	SDL_RenderPresent(renderer);
	
}



