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

// Definitions
#define XBEE_BAUDRATE 9600
#define COMMAND_COMMODETIMEOUT "ATCT0x1770"
#define COMMAND_FWVERSION "ATCH" 
#define COMMAND_APPLYCHANGE "ATAC"


// Function prototypes

// Commands.c
char CheckKeyboard(void);
void WaitForExit(void);

// XBEE.cpp
void OpenSerialPort(int portNum);
void SetATCommandMode(void);
void ReadFirmwareVersion(void);
void CloseSerialPort(void); 

// Setting global serial port
extern CSerial serial;