#include "NodeProgram.h"
 using namespace std;


NodeEntry NodeTable[MAX_NUM_NODES];

void FindNeighbors (void)
{
    char command[12];
    char response[MAX_NUM_NODES*FN_RESPONSE_SIZE];
    int read_timeout, i;// read timeout in ms
    char nextByte;
    clock_t timeout;
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

    // Try reading response one character at a time
    timeout = clock() + 1000000; // Give it 10 sec to respond

    numBytesRead = 0;
    while (clock() < timeout)
    {
        numBytesRead += serial.CustomRead(&response[numBytesRead], 47);
        cout << numBytesRead << "Bytes read" << endl;
        usleep(10000); // Only poll every 10 ms
        if(numBytesRead > 46)
            break;
    }
    cout << numBytesRead << "Bytes read" << endl;

}

