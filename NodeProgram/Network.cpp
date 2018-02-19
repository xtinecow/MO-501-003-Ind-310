#include "NodeProgram.h"
 using namespace std;


NodeEntry NodeTable[MAX_NUM_NODES];
unsigned char HostMAC[8];
unsigned char NodeMAC[8];


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
    unsigned char *commandPointer;
    int numBytesRead = 0;


    while(numBytesRead < NETWORK_REQUEST_SIZE)
    {
        usleep(1000000); // Wait 1ms before checking
        // Check for a little bit more than size in case of garbage carriage returns
        numBytesRead += serial.CustomRead(&response[numBytesRead], 3+NETWORK_REQUEST_SIZE);
        printf("Waiting for table request...\n");
    }

    // Look for start of request (0x7e) to align data
    commandPointer = (unsigned char*)response;
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

    // Copy MAC of requester. Doesn't work because of sign corruption.
//    for(i=0; i<8; i++)
//        HostMAC[i] = commandPointer[REQUEST_MAC_OFFSET+i];

    // Now look for payload. Only care about first 2 bytes for header(1, 2)
    if(commandPointer[REQUEST_PAYLOAD_OFFSET] == 1 && commandPointer[REQUEST_PAYLOAD_OFFSET+1] == 2)
    {
        // Undo byte splitting to get MAC from rest of payload (next 16 bytes)
        CombineByteArray(&commandPointer[REQUEST_PAYLOAD_OFFSET+2],HostMAC, 8);

        // HostMAC seems to be corrupted. Hardcode for now.
        cout << "HostMAC: ";
        for(i=0; i<8; i++)
            cout << (int)HostMAC[i] << " ";
        HostMAC[0] = 0x00;
        HostMAC[1] = 0x13;
        HostMAC[2] = 0xA2;
        HostMAC[3] = 0x00;
        HostMAC[4] = 0x41;
        HostMAC[5] = 0x05;
        HostMAC[6] = 0xE6;
        HostMAC[7] = 0x14;

        cout << endl << "Actual MAC: ";
        for(i=0; i<8; i++)
            cout << (int)HostMAC[i] << " ";
        SendTableFrame();



    }


}


void SendTableFrame(void)
{
	int i, nBytesSent;
	TxFrame frame;
	char* framePointer;
	string message;

	frame.delim = 0x7E;
	frame.length[0] = 0; // Way less than 1 byte so MSB is always 0
	frame.length[1] = sizeof(TxFrame) - 4; // 4 header bytes not counted? (Based on XCTU Frame Generator)
	frame.type = 0x10; // Request type
	frame.ID = 1;
	for (i = 0; i < 8; i++)
		frame.MAC[i] = HostMAC[i];

	frame.FFFE[0] = 0xFF;
	frame.FFFE[1] = 0xFE;
	frame.broadcast = 0;
	frame.option = 0;
	frame.payload[0] = 1; // Just try sending a 3 for now
	frame.payload[1] = 2; // Just try sending a 3 for now
	frame.payload[2] = 3; // Just try sending a 3 for now
	frame.payload[3] = 4; // Just try sending a 3 for now
	frame.payload[4] = 5; // Just try sending a 3 for now


	CalculateFrameChecksum(&frame);

	// Send it off
	cout << "Sending request for table... ";
	// Writing a whole block using serial.write doesn't work for some reason
    // serial.write((char*)&frame, sizeof(TxFrame));

    // Seems to only work if I append it to a string first
    framePointer = (char*)&frame;
    for (i=0; i<sizeof(TxFrame); i++)
    {
        message.append(1, *framePointer);
        framePointer++;
    }
        serial << message;
    cout << "done." << endl;
}
