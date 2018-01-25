#include "NodeProgram.h"

using namespace std;

int main(int argc, char *argv[])
{
    OpenXbeeConnection();
    SetATCommandMode();
    FindNeighbors();

    // Close serial port
    serial.Close();

}
