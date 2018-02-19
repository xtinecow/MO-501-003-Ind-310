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

//Format is as follows:
// 7E
// Length MSB
// Length LSB
// 90 (Frame type)
// MAC[8]
// FFFE
// c1
// Payload[18]
// Checksum
#define NETWORK_REQUEST_SIZE 34
#define REQUEST_MAC_OFFSET 4
#define REQUEST_PAYLOAD_OFFSET 15

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
void GetNodeMAC(void);
void CheckForOK (void);
void ApplyChange(void);


//Network.cpp
void FindNeighbors (void);
void SetNetworkID (void);
void WaitForNetworkCommand(void);
void SendTableFrame(void);

//Utilities.cpp
int ConvertHexByteToInt (char* pointer);
void ParseFNResponse(char *response, int size);
void DisplayNodeTable(void);
// void CalculateFrameChecksum(TxFrame *frame); // prototype moved after struct
void CombineByteArray(unsigned char *src, unsigned char* dest, int size);

///////////// Structure for node table
struct NodeEntry
{
    char MAC[8];
    int RSSI;
};


///////////// Globals
extern LibSerial::SerialStream serial;
extern NodeEntry NodeTable[MAX_NUM_NODES];
extern unsigned char HostMAC[8];
extern unsigned char NodeMAC[8];



// Frame to send table back to host
struct TxFrame
{
    unsigned char delim;
    unsigned char length[2];
    unsigned char type;
    unsigned char ID;
    unsigned char MAC[8];
    unsigned char FFFE[2];
    unsigned char broadcast;
    unsigned char option;
    // Keep 18 bytes reserved for GNSS info (not currently used):
    // 2 byte flag, 8 byte lat, 8 byte long
    // unsigned char payload[sizeof(NodeTable)+18];
    unsigned char payload[6];
    unsigned char checksum;
};

// Had to move prototype after struct definition for this
void CalculateFrameChecksum(TxFrame *frame);
