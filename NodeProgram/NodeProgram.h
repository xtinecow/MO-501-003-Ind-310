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
#define MAX_NUM_NODES 5 // 5 neighboring nodes maximum

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
void SendTableFrame(int sequence);
void SetFNTimeout(void);

//Utilities.cpp
int ConvertHexByteToInt (char* pointer);
void ParseFNResponse(char *response, int size);
void DisplayNodeTable(void);
// void CalculateFrameChecksum(TxFrame *frame); // prototype moved after struct
void CombineByteArray(unsigned char *src, unsigned char* dest, int size);
void SplitByteArray(unsigned char *src, unsigned char* dest, int size);
void FlushAPIBuffer (void);

///////////// Structure for node table
struct NodeEntry
{
    unsigned char MAC[8];
    short RSSI;
};


///////////// Globals
extern LibSerial::SerialStream serial;
extern NodeEntry NodeTable[MAX_NUM_NODES];
extern unsigned char HostMAC[8];
extern unsigned char NodeMAC[8];



// Frame to send table back to host
// Payload will be split in following way:
// header: 1 byte
// sequence: 1 byte
// NodeMAC: 2*8=16 bytes
// 3 Neighours = 3*(2*8+2) = 54 bytes
// Total = 72 bytes
// For second frame, third neighbor spot will be replaced by GNSS info
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
    unsigned char payload[72];
    // unsigned char payload[6];
    unsigned char checksum;
};

// Had to move prototype after struct definition for this
void CalculateFrameChecksum(TxFrame *frame);
