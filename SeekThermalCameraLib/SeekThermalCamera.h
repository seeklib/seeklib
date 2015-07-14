#pragma once
#ifndef _SEEK_THERMAL_LIB_H
#define _SEEK_THERMAL_LIB_H

//Forward declarations
typedef struct libusb_context libusb_context;
typedef struct libusb_device_handle libusb_device_handle;

/*class CalibratedThermalFrame
{
	private:
		const uint32_t width = 208;
		const uint32_t width = 156;
};*/

///@brief An exception enumeration for the SeekThermalCamera object.
enum class SeekThermalCamera_Exception
{
	Camera_Not_Connected,
	Data_Error,
	Could_Not_Init_LibUSB_Context
};

class SeekThermalCamera
{
	private:
		//The libusb context
		libusb_context			*cxt;
		
		//The Seek Thermal Camera device handle.
		libusb_device_handle	*seek;
		
	public:
		//Constructor (creates a libusb context)
		SeekThermalCamera();
		
		//Destructor (destroys libusb device handle and context if extant)
		~SeekThermalCamera();
		
		//Sends a deinitialization packet to the Seek Thermal Camera.
		void sendDeinit();
		
		//Connects to and initializes the Seek Thermal Camera.
		void initialize();
		
		//Deinitializes and disconnects from the Seek Thermal Camera.
		void deinitialize();
		
		//Grabs a frame and saves it to a buffer from the camera given a buffer and a buffer size.
		//Returns the number of bytes received.
		size_t grabFrame(uint8_t *data, uint32_t size);
};

#endif	//_SEEK_THERMAL_LIB_H