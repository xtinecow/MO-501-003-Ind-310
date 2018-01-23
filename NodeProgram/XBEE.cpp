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
    clock_t timeout;

    int counter =0;

    std::cout << "Putting module in command BLAH mode... ";
    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    for (i=0; i<3; i++)
    {
        serial << '+';
        usleep(50000); // Wait for 50 milisec between each one
    }

    std::cout << "done." << std::endl;

    usleep(1000000); // wait for required guard time (1 000 000 microseconds)

    std::cout << "Waiting for OK Response... ";
    timeout = clock() + 0.5*CLOCKS_PER_SEC; // Give it 1 sec to respond

 //   while(clock() < timeout)
usleep(5000000);
 serial.read(response, 3);
 while(counter < 10)
    {
    std::cout << timeout - clock() << std::endl;
        //serial.read(response, 20);
//        if(response[0]=='O')
//        { std::cout << "Yay!!!!!!";
//        break;
//        }
counter++;
    }
    std::cout << response << std::endl;
//
//int temp = (int)response[0];
//    if(response[0])
//       std::cout << "Response:"<< temp << response[0] << response[1] << response[2];
//    else
//        std::cout << "Error reading response" << std::endl;


}