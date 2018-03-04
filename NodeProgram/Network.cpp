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

    serial << "ATFN\r";

    numBytesRead = 0;
    timeout = 0;

    while (timeout < 1) // timeout didn't seem to be working so just trying one big read for now
    {
        if(usleep(4000000)) // Max time = NT+0.8 = 4 seconds
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
        std:: cout << "Sending table to MAC: ";
        for(i=0; i<7; i++)
            cout << uppercase << setw(2) << setfill('0') << (int)HostMAC[i] << ":";
        cout << uppercase << setw(2) << setfill('0') << (int)HostMAC[7] << endl;

        // Send first 3 nodes
        SendTableFrame(0);
        FlushAPIBuffer();
        // Send last 2
        SendTableFrame(1);
        FlushAPIBuffer();


    }


}


void SendTableFrame(int sequence)
{
	int i, nBytesSent;
	TxFrame frame;
	char* framePointer;
	string message;
	unsigned char temp;

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
	frame.payload[0] = 3; // Table header is a 3
	frame.payload[1] = sequence; // Sequence number
	SplitByteArray(NodeMAC, &frame.payload[2], 8); // Next 8 spots is this node's MAC

	if(!sequence)
	{
        // Send first 3 Nodes the first time around
        for(i=0; i<3; i++)
        {
            // Every iteration adds 16+2 = 18 bytes
            SplitByteArray(NodeTable[i].MAC, &frame.payload[i*18+18], 8); // Copy MAC

            // Need to split RSSI too
            // RSSI is short but only LSB holds info. Split LSB into 2 bytes
            temp =((unsigned char)NodeTable[i].RSSI & 0x00ff);
            SplitByteArray(&temp, &frame.payload[i*18+34], 1);
        }
    }
    else
    {
        // Send last 2 nodes
        for(i=0; i<2; i++)
        {
            // Every iteration adds 16+2 = 18 bytes
            SplitByteArray(NodeTable[3+i].MAC, &frame.payload[i*18+18], 8); // Copy MAC

            // Need to split RSSI too
            // RSSI is short but only LSB holds info. Split LSB into 2 bytes
            temp =((unsigned char)NodeTable[3+i].RSSI & 0x00ff);
            SplitByteArray(&temp, &frame.payload[i*18+34], 1);
        }
    }



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

void SetFNTimeout(void)
{
    cout<<"Setting FN Timeout to 3.2 sec... ";
    serial << "ATNT0x20\r";

    CheckForOK();
    ApplyChange();
}
