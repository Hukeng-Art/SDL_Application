#include "SDL_application.h"



class SDL_application {
	
protected:
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	
	bool is_running;
	
	clock_t start;
	clock_t end;
	float delta;
	
	// Default values - override in subclass
	int window_width = 800;
	int window_height = 500;
	bool window_resizable = true;
	const char* window_title = "Application";
	const char* icon = "assets/icon.png";
	
	// debug vars, delete/comment out later
	int debug_cycle_counter = 0;
	float debug_second_counter = 0;
	
	
public:

	SDL_application() { // Constructor
		
		if (!SDL_Init(SDL_FLAGS)) { // if SDL_Init fails, then...
			throw std::runtime_error("Error initiating SDL.");
		}
		
		
		if (!TTF_Init()) { // if TTF_Init fails, then...
			throw std::runtime_error("Error initiating TTF.");
		}
		
		// SDL_CreateWindow() returns pointer to window
		window = SDL_CreateWindow(window_title, window_width, window_height, 0);
		
		// check if pointer is populated (not null) (alt: if (window == NULL))
		if (!window) {
			throw std::runtime_error("Error creating Window.");
		}
		
		// set window to resizable (true by default)
		SDL_SetWindowResizable(window, window_resizable);
		
		// SDL_CreateRenderer() returns pointer to renderer for window
		renderer = SDL_CreateRenderer(window, NULL);
		
		// check if pointer is populated (not null) (alt: if (renderer == NULL))
		if (!renderer) {
			throw std::runtime_error("Error creating Renderer.");
		}
		
		// set up window icon - destroyed after use to free up memory
		SDL_Surface *icon_surf = IMG_Load(icon);
		if (!icon_surf) {
			throw std::runtime_error("Error loading icon surface.");
		}
		if (!SDL_SetWindowIcon(window, icon_surf)) {
			SDL_DestroySurface(icon_surf); // free up storage even in case of failure
			throw std::runtime_error("Error setting window icon.");
		}
		SDL_DestroySurface(icon_surf); // free storage
		
		is_running = true;
			
	}
	
	
	~SDL_application(){ // DESTRUCTOR - CLEAN UP MEMORY
		
		
		if (renderer) { // if pointer to renderer is available
			SDL_DestroyRenderer(renderer); // free memory at renderer pointer
			renderer = NULL;               // set dangling pointer to null
		}
		
		if (window) { // if pointer to window is available
			SDL_DestroyWindow(window); // free memory at window pointer
			window = NULL;             // set dangling pointer to null
		}
		
		TTF_Quit(); // bring ttf module offline
		SDL_Quit(); // bring sdl module offline
		
	}
	
	void run() {
		
		start = clock();
		
		while(is_running) {
			events();
			update();
			draw();
		}	
	}
	
protected:

	void events() {
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
			
			events_ext(event);
			
		}	
		
	}
	
	void update() {
		
		// TO DO : GET DELTA UPDATE WORKING!
		
		end = clock();
		delta = (float)(end-start) / CLOCKS_PER_SEC; // calculate duration of previous refresh cycle, set delta
		start = clock();
	
		
		SDL_Delay(10);
		
		debug_second_counter += delta;
		debug_cycle_counter++;
		
		
		if (debug_second_counter >= 1) {
			std::cout << "One second elapsed - current speed: " << debug_cycle_counter << " FPS\n";
			debug_cycle_counter = 0;
			debug_second_counter = 0;
		}
		
				
		update_ext();
		
	}
	
	void draw() {
		SDL_RenderClear(renderer); // clear renderer buffer
		
		draw_ext(); // extended draw function for actual application
		
		SDL_RenderPresent(renderer);
		
	}
	
	
	// Virtual functions - extend in child classes
	
	virtual void events_ext(SDL_Event event) {}
	virtual void update_ext() {}
	virtual void draw_ext() {}
	


};
