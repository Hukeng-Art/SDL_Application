#include "SDL_application.cpp"


int main() {
	
	SDL_application* test = new SDL_application(800,500);
	
	test->run();
	
	delete test;
	
	return 0;
}
