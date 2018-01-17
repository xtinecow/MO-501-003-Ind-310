// This file will handle communication with all nodes in the network 


#include"ComputerProgram.h"
using namespace std;

NodeEntry_t NodeList[MAX_NUM_NODES];

// Find all nodes in the network and fill network table with that information
void NetworkDiscover(void)
{
	int nBytesSent, nBytesRead;
	char command[20];
	char response[ND_RESPONSE_SIZE]; 
	clock_t timeout;


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
	timeout = clock() + 15*CLOCKS_PER_SEC; // Give it 15 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, ND_RESPONSE_SIZE);
		if (nBytesRead >= (ND_RESPONSE_SIZE-1))
		{
			cout << "0x" << response[0] << response[1] << response[2] << response[3] << endl;
			cout << "0x" << response[4] << response[5] << response[6] << response[7] << endl;
			cout << "0x" << response[8] << response[9] << endl;
			break; 
		}
		Sleep(100); // Only pull every 100 ms
	}
	cout << nBytesRead << " bytes read" << endl; 
	if (!nBytesRead)
		cout << "No Response" << endl; 
}