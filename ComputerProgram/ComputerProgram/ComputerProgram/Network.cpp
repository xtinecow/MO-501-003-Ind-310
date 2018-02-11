// This file will handle communication with all nodes in the network 


#include"ComputerProgram.h"
using namespace std;

NodeEntry NodeList[MAX_NUM_NODES];

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
	timeout = clock() + 13*CLOCKS_PER_SEC; // Give it 10 sec to respond
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
	command[7] = '0';
	command[8] = '1';
	command[9] = 13; // Add carriage return 

	cout << "Setting network ID to 0x0001... "; 
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 10);
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
		request.MAC[i] = NodeList[0].MAC[i]; 

	request.FFFE[0] = 0xFF; 
	request.FFFE[1] = 0xFE; 
	request.broadcast = 0; 
	request.option = 0; 
	request.payload[0] = 1; // Send a 1 to request table

	CalculateRequestChecksum(&request); 

	// Send it off
	cout << "Sending request for table... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData((char*)&request, sizeof(TxFrame));
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl; 
}