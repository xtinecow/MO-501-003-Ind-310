// This file will handle communication with all nodes in the network 


#include"ComputerProgram.h"
using namespace std;

NodeEntry NodeList[MAX_NUM_NODES];
unsigned char HostMAC[8]; 

// Find all nodes in the network and fill network table with that information
void NetworkDiscover(void)
{
	int nBytesSent, nBytesRead, i;
	char command[20];
	char response[MAX_NUM_NODES*ND_RESPONSE_SIZE];
	clock_t timeout;   



	// First need to set option to display RSSI. This is a 0x04 for NO
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'N';
	command[3] = 'O';
	command[4] = '0';
	command[5] = 'x';
	command[6] = '0';
	command[7] = '4';
	command[8] = 13; // Add carriage return 


	cout << "Setting option to return RSSI... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 9);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl;

	CheckForOKResponse(); 

	ApplyChangeCommand(); 


	// Now send network discover command
	command[0] = 'A';
	command[1] = 'T'; 
	command[2] = 'N'; 
	command[3] = 'D'; 
	command[4] = 13; // Add carriage return 

	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	cout << "Sending Network Discover Command... " << endl; 
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + 4*CLOCKS_PER_SEC; // Give it 4 sec to respond (timeout is 3.2 sec)
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData((char*)&response[nBytesRead], MAX_NUM_NODES*ND_RESPONSE_SIZE);
		if (nBytesRead >= MAX_NUM_NODES*ND_RESPONSE_SIZE)
			break; 
		Sleep(100); // Only poll every 100 ms
	} 


	if (!nBytesRead)
		cout << "No Response" << endl; 
	else
	{
		cout << nBytesRead << " bytes read" << endl; 

		ParseNDResponse(response, nBytesRead);

	}
}

// Computer XBEE module will act as Node 0 and us this to find all its neighbors
// Note that this has the same response size/type as network discover
void FindNeighbors(void)
{
	int nBytesSent, nBytesRead, i;
	char command[20];
	char response[MAX_NUM_NODES*ND_RESPONSE_SIZE];
	clock_t timeout;
	// Now send network discover command
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'F';
	command[3] = 'N';
	command[4] = 13; // Add carriage return 

	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	cout << "Sending Find Neighbor command..."; 
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + 4 * CLOCKS_PER_SEC; // Give it 4 sec to respond (timeout is 3.2 sec)
	nBytesRead = 0;
	while (clock() < timeout)
	{
		// Max number of neighbors is the number of nodes -1
		nBytesRead += serial.ReadData((char*)&response[nBytesRead], (MAX_NUM_NODES-1)*ND_RESPONSE_SIZE);
		if (nBytesRead >= (MAX_NUM_NODES - 1)*ND_RESPONSE_SIZE)
			break;
		Sleep(100); // Only poll every 100 ms
	}

		ParseFNResponse(response, nBytesRead);
		cout << "done" << endl; 

}

void SetNetworkID(void)
{
	char command[12];
	int nBytesRead, nBytesSent;
	char response[12];
	clock_t timeout;

	// Set network ID to 0x0001
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'I';
	command[3] = 'D';
	command[4] = '0';
	command[5] = 'x';
	command[6] = '0';
	command[7] = '0';
	command[8] = '0';
	command[9] = '1';
	command[10] = 13; // Add carriage return 

	cout << "Setting network ID to 0x0001... "; 
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 11);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl;

	CheckForOKResponse(); 

	// Apply change
	ApplyChangeCommand(); 

}

void SendTableRequest(int node)
{
	int i, nBytesSent; 
	TxFrame request;

	request.delim = 0x7E; 
	request.length[0] = 0; // Way less than 1 byte so MSB is always 0
	request.length[1] = sizeof(TxFrame) - 4; // 4 header bytes not counted? (Based on XCTU Frame Generator)
	request.type = 0x10; // Request type
	request.ID = 1; 
	for (i = 0; i < 8; i++)
		request.MAC[i] = NodeList[node].MAC[i]; 

	request.FFFE[0] = 0xFF; 
	request.FFFE[1] = 0xFE; 
	request.broadcast = 0; 
	request.option = 0; 
	request.payload[0] = 1; // Send a 1, 2 to request table
	request.payload[1] = 2;

	// Split HostMAC into rest of payload (16 bytes total) 
	SplitByteArray(HostMAC, &request.payload[2], 8); 


	CalculateRequestChecksum(&request); 

	// Send it off
	cout << "Sending request for table from node " << node << "... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData((char*)&request, sizeof(TxFrame));
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl; 
}


void WaitForTableFrame(int node)
{
	char response[300]; // Just trying to read 10 bytes for now, change it later
	int nBytesRead, i, neighbor; 
	int payloadOffset; 
	clock_t timeout;
	unsigned char sourceMac[8];

	timeout = clock() + 2 * CLOCKS_PER_SEC; // Give it 2 sec to respond
	nBytesRead = 0;
	payloadOffset = 26;
	while (clock() < timeout)
	{
		// First payload: Looking for 99 bytes:
		// First 11 bytes redundant (automatic request response)
		// After that is start of table frame
		// 15 bytes of redundant header info
		// Next 72 bytes is payload 
		// Last is checksum

		// Second payload: 
		// 88 bytes

		// Total: 187 bytes
		nBytesRead += serial.ReadData((char*)&response[nBytesRead], 98);
		if (nBytesRead >= 187)
		{
			// Check payload header of 3 for valid payload. First packet should have sequence of 0. 
			if (response[payloadOffset] == 3 && response[payloadOffset + 1] == 0)
			{
				// First is source MAC address 
				payloadOffset += 2;
				CombineByteArray((unsigned char*)&response[payloadOffset], sourceMac, 8);
				// Now check if it matches expected MAC
				for (i = 0; i < 8; i++)
				{
					if (sourceMac[i] != NodeList[node].MAC[i])
					{
						cout << "Error. Packet came from unexpected node." << endl;
						return;
					}
				}
				payloadOffset += 16;
				// All checks out, copy in table. 
				for (neighbor = 0; neighbor < 3; neighbor++)
				{
					// MAC
					CombineByteArray((unsigned char*)&response[payloadOffset], NodeList[node].NodeTable[neighbor].MAC, 8);
					payloadOffset += 16;
					// RSSI
					CombineByteArray((unsigned char*)&response[payloadOffset], (unsigned char*)&NodeList[node].NodeTable[neighbor].RSSI, 1);
					payloadOffset += 2;
				}
			}
			// Now move to second frame
			payloadOffset = 114;
			// Check payload header of 3 for valid payload. Second frame should have sequence of 1. 
			if (response[payloadOffset] == 3 && response[payloadOffset + 1] == 1)
			{
				// First is source MAC address 
				payloadOffset += 2;
				CombineByteArray((unsigned char*)&response[payloadOffset], sourceMac, 8);
				// Now check if it matches expected MAC
				for (i = 0; i < 8; i++)
				{
					if (sourceMac[i] != NodeList[node].MAC[i])
					{
						cout << "Error. Packet came from unexpected node." << endl;
						return;
					}
				}
				payloadOffset += 16;
				// All checks out, copy in table for last 2 neighbors
				for (neighbor = 0; neighbor < 2; neighbor++)
				{
					// MAC
					CombineByteArray((unsigned char*)&response[payloadOffset], NodeList[node].NodeTable[neighbor + 3].MAC, 8);
					payloadOffset += 16;
					// RSSI
					CombineByteArray((unsigned char*)&response[payloadOffset], (unsigned char*)&NodeList[node].NodeTable[neighbor + 3].RSSI, 1);
					payloadOffset += 2;
				}
			}

			return;
		}
			
		Sleep(100); // Only poll every 100 ms
	}


	 cout << "No valid response. Only " << nBytesRead << "bytes read." << endl;


}

// Set timeout values for ND and FN commands to 3.2 sec 
void SetNetworkTimeout(void)
{
	char command[12];
	int nBytesSent;
	clock_t timeout;

	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'N';
	command[3] = 'T';
	command[4] = '0';
	command[5] = 'x';
	command[6] = '2';
	command[7] = '0';
	command[8] = 13; // Add carriage return 

	cout << "Setting network timeout to 3.2 sec... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 9);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl;

	CheckForOKResponse();

	// Apply change
	ApplyChangeCommand();

}