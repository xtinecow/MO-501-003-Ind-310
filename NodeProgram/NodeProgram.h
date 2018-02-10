// Main header file for the Node Project

/////////////// Add all libraries here
#include <iostream>
#include<time.h>
#include<iomanip>

// Using serial class obtained online from https://github.com/Derecho/libserial/blob/master/src/SerialStream.h
// Tutorial for this found at http://libserial.sourceforge.net/x27.html
#include"SerialStream.h"
#include"SerialStreamBuf.h"


////////////// Definitions
#define MAX_NUM_NODES 6

// Format is as follows:
// FFFE
// MAC (first 4 bytes)
// MAC (second 4 bytes)
// (space)
// FFFE
// Device Type: Router -01
// Status: Reserved - 00
// Profile ID- C105
// Manufacturer ID - 101E
// RSSI
// One last carriage return

// Each line is terminated by a carriage return which takes one byte.
// returned in ASCII so each byte takes two chars
#define FN_RESPONSE_SIZE 50

////////////// Function Prototypes

//XBEE.cpp
void OpenXbeeConnection (void);
void SetATCommandMode (void);
void SetAPIMode(void);
void ExitCommandMode(void);
void CheckForOK (void);
void ApplyChange(void);


//Network.cpp
void FindNeighbors (void);
void SetNetworkID (void);

//Utilities.cpp
int ConvertHexByteToInt (char* pointer);
void ParseFNResponse(char *response, int size);
void DisplayNodeTable(void);

///////////// Structure for node table
struct NodeEntry
{
    char MAC[8];
    int RSSI;
};



///////////// Globals
extern LibSerial::SerialStream serial;
extern NodeEntry NodeTable[MAX_NUM_NODES];
