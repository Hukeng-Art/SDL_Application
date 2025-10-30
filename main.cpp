#include "SDL_application.cpp"


int main() {
	
	SDL_application* test = new SDL_application();
	
	test->run();
	
	delete test;
	
	return 0;
}
