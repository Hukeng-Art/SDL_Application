#include <iostream>
#include <vector>

#include "PibRemoteApp.h"

#include "../Pib/Pib_Tinkerforge/Classes/Robot.cpp"


#define BRICKLET_NUM 3
#define SERVO_DELAY 10
#define FINGER_SPEED_SCALE 3

#define SERVO_SPEED 200
#define BLINK_DURATION 10000
#define INVERSION {{1,1,1,-1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1}}

class PibRemoteApp {
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	
	bool is_running;
	
	clock_t start;
	clock_t end;
	double delta;
	
	int debug_cycle_counter = 0;
	double debug_second_counter = 0;
	
	
	Robot* robot;
	int moving_servos[3][10];
	int servo_positions[3][10];
	int inversion[3][10] = INVERSION;
	
	std::vector<SDL_Texture*> pib_eyes;
	
	int eye_counter;
	int eye_index;
	
	
	// Default values - override in subclass
	int window_width = 800;
	int window_height = 500;
	bool window_resizable = true;
	const char* window_title = "Application";
	const char* icon = "assets/icon.png";
	
	
public:

	PibRemoteApp() { // Constructor
		
		if (!SDL_Init(SDL_FLAGS)) { // if SDL_Init fails, then...
			throw std::runtime_error("Error initiating SDL.");
		}
		
		// SDL_CreateWindow() returns pointer to window
		window = SDL_CreateWindow(window_title, window_width, window_height, 0);
		
		// check if pointer is populated (not null) (alt: if (window == NULL))
		if (!window) {
			throw std::runtime_error("Error creating Window.");
		}
		
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
		
		setup_ext();
			
	}
	
	
	~PibRemoteApp(){ // DESTRUCTOR - CLEAN UP MEMORY
		
		free_ext();
		
		if (renderer) { // if pointer to renderer is available
			SDL_DestroyRenderer(renderer); // free memory at renderer pointer
			renderer = NULL;               // set dangling pointer to null
		}
		
		if (window) { // if pointer to window is available
			SDL_DestroyWindow(window); // free memory at window pointer
			window = NULL;             // set dangling pointer to null
		}
		
		
		for (SDL_Texture* texture : pib_eyes) {
				if (texture) { // if pointer to background texture is available
				SDL_DestroyTexture(texture);  // free memory at background texture pointer
				texture = NULL;               // set dangling pointer to null
			}
			
		}
		
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
	
private:

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
				
		update_ext();
		
	}
	
	void draw() {
		SDL_RenderClear(renderer); // clear renderer buffer
		
		draw_ext(); // extended draw function for actual application
		
		SDL_RenderPresent(renderer);
		
	}
	
	
	// Virtual functions - extend in child classes
	
	void events_ext(SDL_Event event) {
		switch (event.type) {
			case SDL_EVENT_KEY_DOWN:
				
				switch (event.key.scancode) {
					// shoulder
					case SDL_SCANCODE_E:
						moving_servos[1][0] = 1;
						break;
					case SDL_SCANCODE_Q:
						moving_servos[1][0] = -1;
						break;
					case SDL_SCANCODE_S:
						moving_servos[1][1] = 1;
						break;
					case SDL_SCANCODE_W:
						moving_servos[1][1] = -1;
						break;
					//upper arm
					case SDL_SCANCODE_D:
						moving_servos[0][9] = 1;
						break;
					case SDL_SCANCODE_A:
						moving_servos[0][9] = -1;
						break;
					// elbow
					case SDL_SCANCODE_I:
						moving_servos[0][8] = 1;
						break;
					case SDL_SCANCODE_K:
						moving_servos[0][8] = -1;
						break;
					// wrist rotation
					case SDL_SCANCODE_L:
						moving_servos[0][7] = 1;
						break;
					case SDL_SCANCODE_J:
						moving_servos[0][7] = -1;
						break;
					// fingers
					case SDL_SCANCODE_P:
						moving_servos[0][2] = FINGER_SPEED_SCALE;
						moving_servos[0][3] = FINGER_SPEED_SCALE;
						moving_servos[0][4] = FINGER_SPEED_SCALE;
						moving_servos[0][5] = FINGER_SPEED_SCALE;
						break;
					case SDL_SCANCODE_O:
						moving_servos[0][2] = -1 * FINGER_SPEED_SCALE;
						moving_servos[0][3] = -1 * FINGER_SPEED_SCALE;
						moving_servos[0][4] = -1 * FINGER_SPEED_SCALE;
						moving_servos[0][5] = -1 * FINGER_SPEED_SCALE;
						break;
					//thumb
					case SDL_SCANCODE_H:
						moving_servos[0][0] = FINGER_SPEED_SCALE;;
						moving_servos[0][1] = FINGER_SPEED_SCALE;;
						break;
					case SDL_SCANCODE_U:
						moving_servos[0][0] = -1 * FINGER_SPEED_SCALE;;
						moving_servos[0][1] = -1 * FINGER_SPEED_SCALE;;
						break;
					default:
						break;
				}
			
			default:
				break;
		}
	}
	
	void update_ext() {
		
		for (int i = 0; i < BRICKLET_NUM; i++) {
			for (int j = 0; j < 10; j++) {
				if (moving_servos[i][j] != 0) {
					
					servo_positions[i][j] += SERVO_SPEED * inversion[i][j] * moving_servos[i][j];
					
					// putting everything in one statement causes unexpected behavior
					if (servo_positions[i][j] > 9000) {
						servo_positions[i][j] = 9000;
					} else if (servo_positions[i][j] < -9000) {
						servo_positions[i][j] = -9000;
					}
					
					robot->servos->set_servo_pos(i, j, servo_positions[i][j]);
					moving_servos[i][j] = 0;
					//SDL_Delay(SERVO_DELAY);
			
				}
				
			}
			
		}
			
		if (eye_counter == BLINK_DURATION) {
			eye_counter = 0;
			eye_index = (eye_index + 1) % pib_eyes.size();
		} else {
			eye_counter++;
		}
	}
	
	void draw_ext() {
	
		// last two params: pointers to source frect and target frect, null for full image & full screen
		SDL_RenderTexture(renderer, pib_eyes[eye_index], NULL, NULL);
	
	}
	
	void setup_ext() {
		robot = new Robot("a");
		
		robot->servos->set_servo_pos(0, 8, -4500);
		robot->servos->set_servo_pos(2, 8, -4500);
		
		
		pib_eyes.push_back(IMG_LoadTexture(renderer,"assets/pibEyes/eyes01.png"));
		pib_eyes.push_back(IMG_LoadTexture(renderer,"assets/pibEyes/eyes02.png"));
		pib_eyes.push_back(IMG_LoadTexture(renderer,"assets/pibEyes/eyes03.png"));
		pib_eyes.push_back(IMG_LoadTexture(renderer,"assets/pibEyes/eyes04.png"));
		
		eye_counter = 0;
		eye_index = 0;
		
	}
	
	void free_ext() {
		
		delete robot;
		
	}

};


