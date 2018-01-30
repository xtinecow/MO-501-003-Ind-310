#include "NodeProgram.h"
 using namespace std;


NodeEntry NodeTable[MAX_NUM_NODES];

void FindNeighbors (void)
{
    char command[12];
    char response[MAX_NUM_NODES*FN_RESPONSE_SIZE];
    int read_timeout, i;// read timeout in ms
    char nextByte;
    int timeout;
    int numBytesRead = 0;



	cout << "Setting option to return RSSI...  ";
    serial << "ATNO0x04\r";

    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    std::cout << response << std::endl;
    std:: cout << "Number of bytes read: " << numBytesRead << std::endl;



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
    int numBytesRead;
    char response[12];
    cout<<"Setting network ID to 0x0001... ";
    serial << "ATID0x0001\r";

    // Check for response.
    numBytesRead = 0;
    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    cout << response << endl;

    cout << "Applying change... ";
    serial << "ATAC\r";

    // Check for OK
    numBytesRead = 0;
    while(numBytesRead < 3)
    {
        usleep(1000); // Wait 1ms before checking
        numBytesRead += serial.CustomRead(&response[numBytesRead], 20);
    }
    cout << response << endl;



}
