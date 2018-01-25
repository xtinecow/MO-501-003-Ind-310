// This file will handle communication with the XBEE module
#include"NodeProgram.h"

LibSerial::SerialStream serial;
void OpenXbeeConnection (void)
{
    // Open USB port 0 (top middle usb port on Raspberry PI)
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

    std::cout << "done." << std::endl;

    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    std::cout << "Waiting for OK Response... ";

    usleep(1000); // Wait 1ms before checking
   numBytesRead = serial.CustomRead(response, 20);
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl;

    std::cout<<"Now setting timeout to max value... ";
    serial << "ATCT0X1770\r";

    // Check for response.
    numBytesRead = 0;
    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl;

    std:: cout << "Applying change... ";
    serial << "ATAC\r";

    // Check for OK
    numBytesRead = 0;
    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl;


    std:: cout << "Checking timeout register... ";
    serial << "ATCT\r";

    // Check for response.
    numBytesRead = 0;
    while(numBytesRead < 4)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl << std::endl;


}

