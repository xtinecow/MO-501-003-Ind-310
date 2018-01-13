// This file will handle all communication with the XBEE module 

#include"ComputerProgram.h"
using namespace std; 

CSerial serial;


void OpenSerialPort(int portNum)
{
	if (serial.Open(portNum, XBEE_BAUDRATE))
		cout << "Port successfully opened" << endl;
	else
	{
		cout << "Failed to open port " << portNum << endl;
		WaitForExit();
	}
}

void SetATCommandMode(void)
{
	int nBytesSent, nBytesRead, i; 
	clock_t timeout;
	char response[10];
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

	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + 2*CLOCKS_PER_SEC; // Give it 2 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 2);
		if (nBytesRead)
		{
			cout << response[0] << response[1] << endl;
			break; 
		}
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port"<< endl;
		WaitForExit();
	}

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

	cout << "Waiting fo acknowledgment... ";
	timeout = clock() + 2 * CLOCKS_PER_SEC; // Give it 2 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 1);
		if (nBytesRead)
			break; 
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port" << endl;
		WaitForExit();
	}
	else
		cout << "done." << endl; 

}

void ReadFirmwareVersion(void)
{
	int nBytesSent, nBytesRead; 
	char *response = new char[2]; 
	char message[12] = COMMAND_FWVERSION; 
	clock_t timeout; 
	cout << "Reading version... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(COMMAND_FWVERSION, strlen(COMMAND_FWVERSION));
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + 2*CLOCKS_PER_SEC; // Give it 2 sec to respond
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