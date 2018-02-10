#include "NodeProgram.h"

using namespace std;

int main(int argc, char *argv[])
{
     char response[20];
     int numBytesRead = 0;

    OpenXbeeConnection();
    SetATCommandMode();

    SetNetworkID();
    FindNeighbors();

    std::cout << std::endl << std::endl;

    // Set back to command mode because Find Neighbors will cause it to time out
    SetATCommandMode();
    SetAPIMode();
    ExitCommandMode();



 while(numBytesRead < 3)
    {
        usleep(1000000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 4);
        printf("Waiting for table request...\n");
    }
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl;




    // Close serial port
    serial.Close();

}
