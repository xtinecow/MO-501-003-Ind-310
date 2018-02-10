#include "NodeProgram.h"
 using namespace std;


NodeEntry NodeTable[MAX_NUM_NODES];

void FindNeighbors (void)
{
    char response[MAX_NUM_NODES*FN_RESPONSE_SIZE];
    int timeout;
    int numBytesRead = 0;



	cout << "Setting option to return RSSI...  ";
    serial << "ATNO0x04\r";

    CheckForOK();

   cout << "Now sending find neighbors command... " << endl;

    serial << "ATND\r";

    numBytesRead = 0;
    timeout = 0;

    while (timeout < 1) // timeout didn't seem to be working so just trying one big read for now
    {
        if(usleep(13000000)) // Max time = NT= 13 seconds(default). Might want to make this shorter in the future.
            cout << "sleep failed" << endl;
        numBytesRead += serial.CustomRead(&response[numBytesRead], MAX_NUM_NODES*FN_RESPONSE_SIZE);// Only try reading 2 at a time to see if this makes it better
        timeout++; // Timeout will be dictated by number of usleep operations
    }
    cout << numBytesRead << "Bytes read" << endl;

    ParseFNResponse(response, numBytesRead);
    DisplayNodeTable();

}


void SetNetworkID (void)
{
    cout<<"Setting network ID to 0x0001... ";
    serial << "ATID0x0001\r";

    CheckForOK();
    ApplyChange();

}
