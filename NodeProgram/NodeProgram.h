// Main header file for the Node Project

/////////////// Add all libraries here
#include <iostream>
#include<time.h>

// Using serial class obtained online from https://github.com/Derecho/libserial/blob/master/src/SerialStream.h
// Tutorial for this found at http://libserial.sourceforge.net/x27.html
#include"SerialStream.h"
#include"SerialStreamBuf.h"

////////////// Function Prototypes

//XBEE.cpp
void OpenXbeeConnection (void);
void SetATCommandMode (void);

// Globals
extern LibSerial::SerialStream serial;
