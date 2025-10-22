
#include "PibRemoteApp.cpp"




int main() {

	PibRemoteApp* remote = new PibRemoteApp();
	
	remote->run();
	
	delete remote;
	
	return 0;
	
} 
