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

	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, 20);
		if (nBytesRead == 3)
		{
			cout << response[0] << response[1] << endl;
			break;
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port" << endl;
		WaitForExit();
	}

	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'A';
	command[3] = 'C';
	command[4] = 13; // Append carriage return
	cout << "Applying change.. ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done." << endl;


	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 20);
		if (nBytesRead == 3)
		{
			cout << response[0] << response[1] << endl;
			break;
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port" << endl;
		WaitForExit();
	}

	// Now send command
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
		nBytesRead += serial.ReadData((char*)&response[nBytesRead],ND_RESPONSE_SIZE);
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

		// Grab RSSI of last hop (This will have to be done in 'Find Neighbors' Step instead but for now it's ok here.

		// Conversion broken because atoi doesn't work on letters. Have to make my own function. 
		NodeList[0].RSSI = atoi((const char*)&response[46]) * 16 + atoi((const char*)&response[47]); // convert from ASCII to num

	}
}