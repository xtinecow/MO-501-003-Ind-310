// Main header file for the project. Add all libraries in here. 

#include<iostream>
#include<string>
#include<windows.h>
#include<tchar.h> 
#include<time.h>
#include<stdlib.h>

// Serial library added from blog post
// https://www.codeguru.com/cpp/i-n/network/serialcommunications/article.php/c2503/CSerial--A-C-Class-for-Serial-Communications.htm
// Instructions on this blog post were followed to set up serial communication
// NOTE: The Serial.h file had to be editted to include the necessary headers for the class.
// The cpp file was also edited to cast the char * elements of szPort and szComParams to their desired variables 
// This seems to compile now. The original files were kept under ComputerProgram/Serial Class for future reference
#include "Serial.h"

///////////////// Definitions
#define XBEE_BAUDRATE 9600
#define MAX_NUM_NODES 6

// Format is as follows:
// FFFE
// MAC (first 4 bytes)
// MAC (second 4 bytes)
// (space)
// FFFE
// Device Type: Router - 01
// Status: Reserved - 00 
// Profile ID - C105
// Manufacturer ID - 101E
// RSSI
// One last carriage return 


// Each line is terminated by a carriage return which takes one byte.
// returned in ASCII so each byte takes two chars. 
#define ND_RESPONSE_SIZE 50


///////////////// Function prototypes

// Commands.c
char CheckKeyboard(void);
void WaitForExit(void);

// XBEE.cpp
void OpenSerialPort(int portNum);
void SetATCommandMode(void);
void ReadFirmwareVersion(void);
void CloseSerialPort(void); 

// Network.cpp
void NetworkDiscover(void);
void SetNetworkID(void);

// Utilities.cpp
void ParseNDResponse(char *response, int size);
int ConvertHexByteToInt(char* pointer);
void DisplayNodeList(void); 

/////////////// Structure for node table 
struct NodeEntry
{
	char MAC[16]; //MAC stored in chars so each byte shows up as 2 chars (just how it's returned, can change later)
	int RSSI; // Only trying this for now. Will eventually have to be table of neighbors. 

};


/////////////// Globals
extern CSerial serial;
extern NodeEntry NodeList[MAX_NUM_NODES];