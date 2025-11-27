#include "SDL_Application.cpp"


int main() {
	
	SDL_Application* test = new SDL_Application();
	
	test->run();
	
	delete test;
	
	return 0;
}
