#include "SeekThermalCamera.h"
#include <signal.h>

bool shouldQuit = false;

void signal_handler(int signum)
{
	shouldQuit = true;
}

int main()
{
	//Set signal handler function.
	signal(SIGINT, signal_handler);
	
	SeekThermalCamera camera;
	
	//Initialize camera.
	camera.initialize();
	
	//Create array for frame data.
	uint8_t frameData[0x7ec0 * 2];
	
	//Loop though data.
	while(!shouldQuit)
	{
		uint32_t bytes_recieved = camera.grabFrame(frameData, 0x7ec0 * 2) != 0);
		
		std::cout << "Bytes received: " << bytes_recieved << std::endl;
	}
	
	//This signal handler is important, because SIGINT would traditionally cause the program to halt abnormally, never deinitializing the USB device.
	//The SeekThermalCamera object destructor handles this automatically when it falls out of scope.
	//Though if it were allocated on the heap with the "new" keyword, delete would need to be called on the pointer.
	
	return EXIT_SUCCESS;
}