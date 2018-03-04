#include "NodeProgram.h"

using namespace std;

int main(int argc, char *argv[])
{

    OpenXbeeConnection();
    SetATCommandMode();
    GetNodeMAC();
    SetNetworkID();
    SetFNTimeout();
    FindNeighbors();

    std::cout << std::endl << std::endl;

    // Set back to command mode because Find Neighbors will cause it to time out
    SetATCommandMode();
    SetAPIMode();
    ExitCommandMode();

    while(1)
    {
        WaitForNetworkCommand();

        // Now update neighbor info
        SetATCommandMode();
        FindNeighbors();
        SetATCommandMode();
        SetAPIMode();
        ExitCommandMode();
    }


    // Close serial port
    serial.Close();

}
