// Main header file for the project. Add all libraries in here. 

#include<iostream>
#include<string>
#include<windows.h>
#include<tchar.h> 
#include<time.h>
#include<stdlib.h>
#include<iomanip>

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

/////////////// Struct Definitions

// Node info for each node
struct NodeInfo
{
	unsigned char MAC[8]; 
	int RSSI; 

};

// Each entry will contain the node's MAC and a table of its neighbors
struct NodeEntry
{
	unsigned char MAC[8];
	NodeInfo NodeTable[MAX_NUM_NODES-1]; 
};

// Frame format for API transmission
// Big endian so no byte swapping necessary
struct TxFrame
{
	unsigned char  delim;
	unsigned char length[2];
	unsigned char type;
	unsigned char ID;
	unsigned char MAC[8];
	unsigned char FFFE[2]; // Reserved, always set to 0xfffe
	unsigned char broadcast;
	unsigned char option;
	unsigned char payload[18]; // Small payload for Tx. Note: Max payload size fixed at 73
	unsigned char checksum;

}; // 24 bytes


///////////////// Function prototypes

// ComputerProgram.cpp
void RunProgram(void);

// Commands.c
char CheckKeyboard(void);
void WaitForExit(void);

// XBEE.cpp
void OpenSerialPort(int portNum);
void SetATCommandMode(void);
void ReadFirmwareVersion(void);
void CheckForOKResponse(void);
void ApplyChangeCommand(void);
void SetAPIMode(void); 
void ExitCommandMode(void); 
void CloseSerialPort(void); 
void GetHostMAC(void); 

// Network.cpp
void NetworkDiscover(void);
void FindNeighbors(void);
void SetNetworkID(void);
void SendTableRequest(int node);
void WaitForTableFrame(int node); 
void SetNetworkTimeout(void);

// Utilities.cpp
void ParseNDResponse(char *response, int size);
void ParseFNResponse(char *response, int size);
int ConvertHexByteToInt(char* pointer);
void DisplayNodeList(void); 
void CalculateRequestChecksum(TxFrame *request);
void SplitByteArray(unsigned char *src, unsigned char* dest, int size);
void CombineByteArray(unsigned char *src, unsigned char* dest, int size); 
void ClearScreen(void);
std::string GetNameFromMAC(unsigned char* MAC);


// TCP.cpp
int SetupTCPSocket(void);
void CloseTCPSocket(void);
void SendNodesToSocket(void);


/////////////// Globals
extern CSerial serial;
extern NodeEntry NodeList[MAX_NUM_NODES]; // List of all the nodes and their tables
extern unsigned char HostMAC[8]; 
extern int setupComplete; 
extern int globalErrorCount; 