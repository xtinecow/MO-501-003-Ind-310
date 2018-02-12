#include "NodeProgram.h"
 using namespace std;


NodeEntry NodeTable[MAX_NUM_NODES];
char HostMAC[8];


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

void WaitForNetworkCommand(void)
{
    int i;
    char response[2*NETWORK_REQUEST_SIZE];
    char *commandPointer;
    int numBytesRead = 0;


    while(numBytesRead < NETWORK_REQUEST_SIZE)
    {
        usleep(1000000); // Wait 1ms before checking
        // Check for a little bit more than size in case of garbage carriage returns
        numBytesRead += serial.CustomRead(&response[numBytesRead], 3+NETWORK_REQUEST_SIZE);
        printf("Waiting for table request...\n");
    }

    // Look for start of request (0x7e) to align data
    commandPointer = response;
    i=0;
    while(*commandPointer != 0x7e)
    {
        commandPointer++;
        i++;
        if(i==3)
        {
            cout << "Request start not found" << endl;
            return;
        }
    }

    // Copy MAC of requester
    for(i=0; i<8; i++)
        HostMAC[i] = commandPointer[REQUEST_MAC_OFFSET+i];

    // Now look for payload. Only care about first byte.
    if(commandPointer[REQUEST_PAYLOAD_OFFSET] == 1)
        cout << "Table Requested!!!" << endl;


}
