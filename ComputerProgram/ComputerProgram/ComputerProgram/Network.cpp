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
	int macIndex, responseIndex; 


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
	timeout = clock() + 10*CLOCKS_PER_SEC; // Give it 15 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response,ND_RESPONSE_SIZE);
		if (nBytesRead >= ND_RESPONSE_SIZE)
			break; 
		Sleep(100); // Only poll every 100 ms
	} 
	if (!nBytesRead)
		cout << "No Response" << endl; 
	else
	{
		cout << nBytesRead << " bytes read" << endl; 
		for (i = 0; i < 8; i++)
			NodeList[0].MAC[i] = response[5+i]; // Copy first half
		for (i = 0; i < 8; i++)
			NodeList[0].MAC[8+i] = response[14+i]; // Skip carriage return and copy second half

	}
}