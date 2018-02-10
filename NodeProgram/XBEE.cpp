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
    char command[20];
    char response[20];

    int numBytesRead =0;

    std::cout << "Putting module in command mode... ";
    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    for (i=0; i<3; i++)
    {
        serial << '+';
        usleep(50000); // Wait for 50 milisec between each one
    }

    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    CheckForOK();

    std::cout<<"Now setting timeout to max value... " << std::endl;
    serial << "ATCT0X1770\r";

    CheckForOK();
    ApplyChange();

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

// Checks serial port for OK response
// Clean up code to always use this function later.
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
// Clean up code to always use this function later.
void ApplyChange(void)
{
    std::cout << "Applying change..." << std::endl;
    serial << "ATAC\r";

    CheckForOK();
}
