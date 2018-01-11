// This file will handle all communication with the XBEE module 

#include"ComputerProgram.h"
using namespace std; 

void SetATCommandMode(CSerial serial)
{
	int nBytesSent, i; 
	// Put device in command mode by sending 3 consecutive '+'commands
	cout << "Putting module in command mode... ";
	Sleep(1000); // wait for the required guard time (default of 1000 ms)
	for (i = 1; i <= 3; i++)
	{
		nBytesSent = 0;
		nBytesSent = serial.SendData("+", 1);
		if (!nBytesSent)
		{
			cout << "Error writing to serial port" << endl;
			WaitForExit(); 
		}
		Sleep(50); 
	}
	cout << "done." << endl;
	Sleep(1000); // wait for the required guard time (default of 1000 ms)


	// Set command mode timeout to max value
	cout << "Setting command mode timeout to max value... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(COMMAND_COMMODETIMEOUT, strlen(COMMAND_COMMODETIMEOUT));
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl; 

}

void ReadFirmwareVersion(CSerial serial)
{
	int nBytesSent, nBytesRead; 
	char *response = new char[2]; 
	char message[12] = COMMAND_FWVERSION; 
	clock_t timeout; 
	cout << "Reading version... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(message, 12);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + 10*CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0; 
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 2);
		if (nBytesRead)
		{
			cout << response[1] << response[2] << endl;
			delete[]response; 
			return; 
		}
	}
	cout << "Message is: " << (string)message << endl; 
	cout << "Error reading from serial port" << endl;
	delete[]response;
	WaitForExit();

}