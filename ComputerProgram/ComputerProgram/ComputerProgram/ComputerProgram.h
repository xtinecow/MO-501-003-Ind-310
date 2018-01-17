// Main header file for the project. Add all libraries in here. 

#include<iostream>
#include<string>
#include<windows.h>
#include<tchar.h> 
#include<time.h>

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

//2 byte 0xfffe
//2 byte parent network address 
//1 byte device type
//2 byte profile ID
//2 byte manufacturer ID
//1 byte RSSI
#define ND_RESPONSE_SIZE 11 


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

/////////////// Structure for node table 
typedef struct
{
	int ID; 

}NodeEntry_t;


/////////////// Globals
extern CSerial serial;
extern NodeEntry_t NodeList[MAX_NUM_NODES];