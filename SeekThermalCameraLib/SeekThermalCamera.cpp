#include "SeekThermalCamera.h"
#include <libusb-1.0/libusb.h>

//TODO:
//Should remove this include after removing the debug statement in grabFrame()
#include <iostream>



SeekThermalCamera::SeekThermalCamera()
{
	seek = nullptr;
	
	//Create the libusb library session.
	int retval = libusb_init(&cxt);
	
	//Check for problems initializuing the libusb context.
	if(r < 0)
		throw SeekThermalCamera_Exception::Could_Not_Init_LibUSB_Context;
}

SeekThermalCamera::~SeekThermalCamera()
{
	if(seek)
	{
		//This will deinitialize and delete the device handle, setting its pointer to null.
		deinitialize();
	}
	
	if(ctx)
	{
		//Close libusb library session.
		libusb_exit(ctx);
		
		ctx = nullptr;
	}
}

void SeekThermalCamera::sendDeinit()
{
	uint8_t out[] = { 0x00, 0x00 };
	libusb_control_transfer(dev, 0x41, 0x3c, 0, 0, out, 2, TIMEOUT_VALUE);
}

void SeekThermalCamera::deinitialize()
{
	if(seek)
	{
		sendDeinit();
		
		libusb_close(seek);
		
		seek = nullptr;
	}
}

void SeekThermalCamera::initialize()
{
	//Check to ensure camera is not already 
	if(!seek)
	{
		//Attempt to open a device handle.
		seek = libusb_open_device_with_vid_pid(ctx, 10397, 16);
	
		//If we fail, the camera is likely not connected.
		if(!seek)
			throw SeekThermalCamera_Exception::Camera_Not_Connected;
		
		unsigned char arr[] = {0x01};

		//Keep trying!
		while(libusb_control_transfer(dev, 0x41, 0x54, 0, 0, arr, 1, TIMEOUT_VALUE) != 1)
		{
			sendDeinit();
		}


		//Send bytes
		unsigned char arr2[] = { 0x00, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x3c, 0, 0, arr2, 2, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data1[4];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x4e, 0, 0, data1, 4, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data2[12];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x36, 0, 0, data2, 12, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr3[] = { 0x20, 0x00, 0x30, 0x00, 0x00, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x56, 0, 0, arr3, 6, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data3[0x40];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x58, 0, 0, data3, 0x40, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr4[] = { 0x20, 0x00, 0x50, 0x00, 0x00, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x56, 0, 0, arr4, 6, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data4[0x40];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x58, 0, 0, data4, 0x40, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr5[] = { 0x0C, 0x00, 0x70, 0x00, 0x00, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x56, 0, 0, arr5, 6, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data5[0x18];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x58, 0, 0, data5, 0x18, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr6[] = { 0x06, 0x00, 0x08, 0x00, 0x00, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x56, 0, 0, arr6, 6, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data6[0x0c];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x58, 0, 0, data6, 0x0c, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr7[] = { 0x08, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x3E, 0, 0, arr7, 2, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data7[2];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x3D, 0, 0, data7, 2, TIMEOUT_VALUE);


		//Send bytes
		unsigned char arr8[] = { 0x08, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x3E, 0, 0, arr8, 2, TIMEOUT_VALUE);

		//Send bytes
		unsigned char arr9[] = { 0x01, 0x00 };
		libusb_control_transfer(dev, 0x41, 0x3C, 0, 0, arr9, 2, TIMEOUT_VALUE);

		//Receive bytes
		uint8_t data8[2];
		libusb_control_transfer(dev, 0xC1 | CONTROL_DIRECTION_IN, 0x3D, 0, 0, data8, 2, TIMEOUT_VALUE);
	}
}

uint32_t SeekThermalCamera::grabFrame(uint8_t *data, uint32_t size)
{
	uint8_t out[] = { 0xc0, 0x7e, 0, 0 };
    libusb_control_transfer(dev, 0x41, 0x53, 0, 0, out, 4, TIMEOUT_VALUE);

    ////device.ReadExactPipe(0x81, 0x7ec0 * 2);

    //uint8_t frame[0x7ec0 * 2];

	//Size should be (0x7ec0 * 2).
	
    int numBytes = 0;
    libusb_bulk_transfer(dev, 0x81, data, size, &numBytes, TIMEOUT_VALUE);

    std::cout << "Got frame! (" << numBytes << " bytes)\n";
	
	return numBytes;
}