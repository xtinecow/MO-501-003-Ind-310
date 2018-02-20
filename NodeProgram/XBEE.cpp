// This file will handle communication with the XBEE module
#include"NodeProgram.h"

LibSerial::SerialStream serial;
void OpenXbeeConnection (void)
{
    // Open USB port 0
     serial.Open("/dev/interceptPtty");
    // serial.Open("/dev/ttyUSB0");

    // Set Baud rate to 9600 (Should already be there by default)
    serial.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);

    // All other options set to default
}

// Basically the same code as the matching function on the computer program
void SetATCommandMode (void)
{
    int i;

    std::cout << "Putting module in command mode... ";
    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    for (i=0; i<3; i++)
    {
        serial << '+';
        usleep(50000); // Wait for 50 milisec between each one
    }

    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    CheckForOK();

//   Comment this out for now to speed it up. Don't think it was necessary
//    std::cout<<"Now setting timeout to max value... " << std::endl;
//    serial << "ATCT0X1770\r";
//
//    CheckForOK();
//    ApplyChange();

}

void SetAPIMode(void)
{

    char response[4];
    int numBytesRead;

    std::cout << "Setting API Mode... " << std::endl;
    serial << "ATAP1\r"; // Set API mode 1
    CheckForOK();
    ApplyChange();



}

void ExitCommandMode(void)
{
    std::cout << "Exiting command mode... " << std::endl;
    serial << "ATCN\r";
    CheckForOK();
}

void GetNodeMAC(void)
{
    int numBytesRead, i;
    char response[12];

    std::cout << "Reading Node MAC... ";

    // Only read lower half since upper half is always the same
    serial << "ATSL\r";
    numBytesRead = 0;

   // Set fixed upper bytes
    NodeMAC[0] = 0x00;
    NodeMAC[1] = 0x13;
    NodeMAC[2] = 0xA2;
    NodeMAC[3] = 0x00;

    while (numBytesRead < 9)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 9);
    }


    // Now grab rest from response
    for(i=0; i<4; i++)
        NodeMAC[4+i] = (unsigned char)ConvertHexByteToInt(&response[2*i]);

    std:: cout << "MAC: ";
    // Print MAC address in hex
    for(i=0; i<7; i++)
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)NodeMAC[i] << ":";
    // Print last one and set format back to decimal
    std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)NodeMAC[7] << std::dec << std::endl;

}

// Checks serial port for OK response
void CheckForOK (void)
{
    char response[4];
    int numBytesRead = 0;
    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 4);
    }
    std::cout << response[0] << response[1] << std::endl;
}

// Applies change to XBEE module
void ApplyChange(void)
{
    std::cout << "Applying change..." << std::endl;
    serial << "ATAC\r";

    CheckForOK();
}
