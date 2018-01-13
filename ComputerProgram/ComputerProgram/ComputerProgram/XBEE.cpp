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
	char response[20];
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
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 20);
		if (nBytesRead)
		{
			cout << response[0] << response[1] << endl;
			// nBytesRead = serial.ReadData(response, 20);
			break; 
		}
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port"<< endl;
		WaitForExit();
	}

	//// Set command mode timeout to max value
	//cout << "Setting command mode timeout to max value... ";
	//nBytesSent = 0;
	//nBytesSent = serial.SendData(COMMAND_COMMODETIMEOUT, strlen(COMMAND_COMMODETIMEOUT));
	//if (!nBytesSent)
	//{
	//	cout << "Error writing to serial port" << endl;
	//	WaitForExit();
	//}
	//cout << "done" << endl; 

	//cout << "Waiting for acknowledgment... ";
	//timeout = clock() + 2 * CLOCKS_PER_SEC; // Give it 2 sec to respond
	//nBytesRead = 0;
	//while (clock() < timeout)
	//{
	//	nBytesRead = serial.ReadData(response, 1);
	//	if (nBytesRead)
	//		break; 
	//}
	//if (!nBytesRead)
	//{
	//	cout << "Error reading from serial port" << endl;
	//	WaitForExit();
	//}
	//else
	//	cout << "done." << endl; 

	//cout << "Applying change.. ";
	//nBytesSent = 0;
	//nBytesSent = serial.SendData(COMMAND_APPLYCHANGE, strlen(COMMAND_APPLYCHANGE));
	//if (!nBytesSent)
	//{
	//	cout << "Error writing to serial port" << endl;
	//	WaitForExit();
	//}
	//cout << "done." << endl;




}

void ReadFirmwareVersion(void)
{
	int nBytesSent, nBytesRead, i; 
	char message[20] = COMMAND_FWVERSION; 
	message[strlen(COMMAND_FWVERSION)] = 13; // Append carriage return to end of command
	 char *messagePointer = message; 


	char response[20];
	clock_t timeout; 
	cout << "Reading version... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(messagePointer, strlen(COMMAND_FWVERSION)+1);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0; 
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 20);
		if (nBytesRead)
		{
			cout << response[0] << response[1] << endl;
			return; 
		}
	}
	cout << "Error reading from serial port" << endl;
	WaitForExit();

}

void CloseSerialPort()
{
	if (serial.Close())
		cout << "Port successfully closed" << endl;
	else
	{
		cout << "Failed to close port "<< endl;
		WaitForExit();
	}
}